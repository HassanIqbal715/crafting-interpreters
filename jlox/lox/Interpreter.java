package lox;

import java.io.IOException;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Scanner;

class Interpreter implements Expr.Visitor<Object>, Stmt.Visitor<Void> {
    final Environment globals = new Environment(this);
    String fileName = "";
    private Environment environment = globals;
    private final Map<Expr, Integer> locals = new HashMap<>();
    public final Map<String, Boolean> importedFiles = new HashMap<>();

    Interpreter() {
        // A native function being defined in the global scope
        globals.define("Clock", new LoxCallable() {
            @Override
            public int arity() { return 0; }

            @Override
            public Object call(Interpreter interpreter, 
                               List<Object> arguments) {
                return (double)System.currentTimeMillis() / 1000.0;
            }

            @Override
            public String toString() { return "<native> fn"; }
        });

        globals.define("input", new LoxCallable() {
            Scanner scanner = new Scanner(System.in);

            @Override
            public int arity() { return 1; }

            @Override
            public Object call(Interpreter interpreter,
                               List<Object> arguments) {
                System.out.print((String) arguments.get(0));
                Object input = null;
                if (scanner.hasNextLine()) {
                    input = scanner.nextLine();
                    return (String) input;
                }
                
                return null;
            }

            @Override
            public String toString() { return "<native> fn"; }
        });

        globals.define("ston", new LoxCallable() {
            @Override
            public int arity() { return 1;} 

            @Override 
            public Object call(Interpreter interpreter, 
                                  List<Object> arguments) {
                try {
                    return Double.parseDouble((String) arguments.get(0));
                }
                catch (NumberFormatException err) {
                    return Double.parseDouble("0");
                }
                catch (NullPointerException err) {
                    return Double.parseDouble("0");
                }
            }

            @Override
            public String toString() { return "<native> fn"; }
        });
    }

    void interpret(List<Stmt> statements) {
        try {
            for (Stmt statement : statements) {
                execute(statement);
            }
        }
        catch (RuntimeError error) {
            Lox.runtimeError(error);
        }
    }

    @Override
    public Object visitLiteralExpr(Expr.Literal expr) {
        return expr.value;
    }

    @Override
    public Object visitLogicalExpr(Expr.Logical expr) {
        Object left = evaluate(expr.left);

        if (expr.operator.type == TokenType.OR) {
            if (isTruthy(left)) return left;
        }
        else {
            if (!isTruthy(left)) return left;
        }

        return evaluate(expr.right);
    }

    @Override
    public Object visitUnaryExpr(Expr.Unary expr) {
        Object right = evaluate(expr.right);
        double value;

        switch(expr.operator.type) {
            case BANG:
                return !isTruthy(right);
            case MINUS:
                checkNumberOperand(expr.operator, right);
                return -(double)right;
            case MINUS_MINUS:
                checkVariableOperand(expr.operator, expr.right);
                value = (double) right - 1;
                environment.assign(((Expr.Variable)expr.right).name, value);
                return value;
            case PLUS_PLUS:
                checkVariableOperand(expr.operator, expr.right);
                value = (double) right + 1;
                environment.assign(((Expr.Variable)expr.right).name, value);
                return value;
            default: break;
        }

        // Unreachable
        return null;
    }

    @Override
    public Object visitPostfixExpr(Expr.Postfix expr) {
        Object value = evaluate(expr.left);

        checkVariableOperand(expr.operator, expr.left);

        double actualValue = (double)value;
        double newValue = (double)value;

        if (expr.operator.type == TokenType.PLUS_PLUS) {
            newValue += 1;
        }
        else if (expr.operator.type == TokenType.MINUS_MINUS) {
            newValue -=1;
        }
        
        environment.assign(((Expr.Variable)expr.left).name, newValue);
        return actualValue;
    }

    @Override
    public Object visitVariableExpr(Expr.Variable expr) {
        return lookUpVariable(expr.name, expr);
    }

    private Object lookUpVariable(Token name, Expr expr) {
        Integer distance = locals.get(expr);
        if (distance != null) {
            return environment.getAt(distance, name.lexeme);
        }
        else {
            return globals.get(name);
        }
    }

    @Override
    public Object visitArrayExpr(Expr.Array expr) {
        List<Integer> indices = new ArrayList<>();
        for (Expr indexExpr : expr.indices) {
            Object indexObject = evaluate(indexExpr);
            checkNumberOperand(expr.name, indexObject);
            indices.add(((Double) indexObject).intValue());
        }
        
        Integer distance = locals.get(expr);
        if (distance != null) {
            return environment.getArrayAt(distance, expr.name, indices);
        }
        else {
            return globals.getArray(expr.name, indices);
        }
    }

    private void checkNumberOperand(Token operator, Object operand) {
        if (operand instanceof Double) return;
        throw new RuntimeError(operator, "Operand must be a number.", 
            fileName);
    }

    private void checkNumberOperands(Token operator, 
                                    Object left, Object right) {
        if (left instanceof Double && right instanceof Double) return;
        throw new RuntimeError(operator, "Operands must be numbers", 
            fileName);
    }

    private void checkVariableOperand(Token operator, Object operand) {
        if (operand instanceof Expr.Variable) return;
        throw new RuntimeError(operator, "Operand must be a variable", 
            fileName);
    }

    private void checkTernaryOperand(Token operator, Object left) {
        if (left instanceof Boolean) return;
        throw new RuntimeError(operator, "Left operand must be a condition"
            , fileName);
    }
    
    private boolean isTruthy(Object object) {
        if (object == null) return false;
        if (object instanceof Boolean) return (boolean)object;
        return true;
    }

    private boolean isEqual(Object a, Object b) {
        if (a == null && b == null) return true;
        if (a == null) return false;

        return a.equals(b);
    }
    
    private String stringify(Object object) {
        if (object == null) return "nil";

        if (object instanceof Double) {
            String text = object.toString();
            if (text.endsWith(".0")) {
                text = text.substring(0, text.length() - 2);
            }
            return text;
        }

        return object.toString();
    }

    @Override
    public Object visitGroupingExpr(Expr.Grouping expr) {
        return evaluate(expr.expression);
    }

    @Override
    public Object visitBinaryExpr(Expr.Binary expr) {
        Object left = evaluate(expr.left);
        Object right = evaluate(expr.right);

        switch(expr.operator.type) {
            case GREATER:
                checkNumberOperands(expr.operator, left, right);
                return (double)left > (double)right;
            case GREATER_EQUAL:
                checkNumberOperands(expr.operator, left, right);
                return (double)left >= (double)right;
            case LESS:
                checkNumberOperands(expr.operator, left, right);
                return (double)left < (double)right;
            case LESS_EQUAL:
                checkNumberOperands(expr.operator, left, right);
                return (double)left <= (double)right;
            case BANG_EQUAL: return !isEqual(left, right);
            case EQUAL_EQUAL: return isEqual(left, right);
            case MINUS:
                checkNumberOperand(expr.operator, right);
                return (double)left - (double)right;
            case PLUS:
                if (left instanceof Double && right instanceof Double) {
                    return (double)left + (double)right;
                }

                if (left instanceof String && right instanceof String) {
                    return (String)left + (String)right;
                }
                
                if (left instanceof String && right instanceof Double) {
                    return (String)left + stringify(right);
                }
                
                if (left instanceof Double && right instanceof String) {
                    return stringify(left) + (String) right;
                }

                throw new RuntimeError(expr.operator,
                    "Operands must be numbers or strings", fileName);
            case SLASH:
                checkNumberOperands(expr.operator, left, right);
                
                if ((double)right == 0.0) {
                    throw new RuntimeError(expr.operator, 
                            "Cannot divide by 0", fileName);
                }

                return (double)left / (double)right;
            case STAR:
                checkNumberOperands(expr.operator, left, right);
                return (double)left * (double)right;
            case MODULO:
                checkNumberOperands(expr.operator, left, right);
                
                if ((double) right == 0.0) {
                    throw new RuntimeError(expr.operator, 
                            "Cannot divide by 0", fileName);
                }

                return (double)left % (double)right;
            case COMMA:
                checkNumberOperand(expr.operator, right);
                return right;
            default: break;
        }
        
        // Unreachable
        return null;
    }

    @Override
    public Object visitCallExpr(Expr.Call expr) {
        Object callee = evaluate(expr.callee);

        List<Object> arguments = new ArrayList<>();
        
        for (Expr argument : expr.arguments) {
            arguments.add(evaluate(argument));
        }

        LoxCallable function = (LoxCallable)callee;
        if (function.arity() != arguments.size()) {
            throw new RuntimeError(expr.paren, "Expected " + function.arity() +
                " arguments but got " + arguments.size() + ".", fileName);
        }

        return function.call(this, arguments);
    }

    @Override
    public Object visitElementsExpr(Expr.Elements expr) {
        List<Expr> elementsExpr = expr.elements;
        List<Object> elements = new ArrayList<>();

        for (Expr elementExpr : elementsExpr) {
            elements.add(evaluate(elementExpr));
        }

        return elements;
    }

    @Override
    public Object visitGetExpr(Expr.Get expr) {
        Object object = evaluate(expr.object);
        if (object instanceof LoxInstance) {
            return ((LoxInstance) object).get(expr.name);
        }

        throw new RuntimeError(expr.name, 
                "Only instances have properties");
    }

    @Override
    public Object visitTernaryExpr(Expr.Ternary expr) {
        Object right = evaluate(expr.right);
        Object mid = evaluate(expr.mid);
        Object left = evaluate(expr.left);
        
        checkTernaryOperand(expr.operator1, left); 
        if ((boolean) left == true) {
            return mid;
        }
        else if ((boolean) left == false) {
            return right;
        }
        
        // Unreachable
        return null;
    }

    private Object evaluate(Expr expr) {
        return expr.accept(this);
    }

    private void execute(Stmt stmt) {
        stmt.accept(this);
    }

    void resolve(Expr expr, int depth) {
        locals.put(expr, depth);
    }

    void executeBlock(List<Stmt> statements, Environment environment) {
        Environment previous = this.environment;
        try {
            this.environment = environment;

            for (Stmt statement : statements) {
                execute(statement);
            }
        } 
        finally {
            this.environment = previous;
        }
    }

    @Override 
    public Void visitArrStmt(Stmt.Arr stmt) {
        Object elements = evaluate(stmt.elements);
        environment.define(stmt.name.lexeme, elements);
        return null;
    }

    @Override
    public Void visitBlockStmt(Stmt.Block stmt) {
        executeBlock(stmt.statements, new Environment(environment, this));
        return null;
    }

    @Override
    public Void visitBreakStmt(Stmt.Break stmt) {
        throw new Exceptions.Break();
    }

    @Override
    public Void visitClassStmt(Stmt.Class stmt) {
        environment.define(stmt.name.lexeme, stmt);
        LoxClass klass = new LoxClass(stmt.name.lexeme);
        environment.assign(stmt.name, klass);
        return null;
    }

    @Override
    public Void visitContinueStmt(Stmt.Continue stmt) {
        throw new Exceptions.Continue();
    }

    @Override
    public Void visitDoStmt(Stmt.Do stmt) {
        try {
            do {
                try {
                    execute(stmt.body);
                }
                catch(Exceptions.Continue exceptionContinue){}
            } while(isTruthy(evaluate(stmt.condition)));
        }
        catch (Exceptions.Break exceptionBreak) {}
        return null;
    }

    @Override
    public Void visitExpressionStmt(Stmt.Expression stmt) {
        evaluate(stmt.expression);
        // System.out.println(stringify(value));
        return null;
    }

    @Override
    public Void visitForStmt(Stmt.For stmt) {
        if (stmt.initializer != null) execute(stmt.initializer);
        try {
            while(isTruthy(evaluate(stmt.condition))) {
                try {
                    execute(stmt.body);
                }
                catch(Exceptions.Continue exceptioContinue) {}
                if (stmt.increment != null) evaluate(stmt.increment);
            }
        }
        catch (Exceptions.Break exceptionBreak) {}

        return null;
    }

    @Override
    public Void visitFunctionStmt(Stmt.Function stmt) {
        LoxFunction function = new LoxFunction(stmt, environment);
        environment.define(stmt.name.lexeme, function);
        return null;
    }

    @Override
    public Void visitIfStmt(Stmt.If stmt) {
        if (isTruthy(evaluate(stmt.condition))) {
            execute(stmt.thenBranch);
        }
        else if (stmt.elseBranch != null) {
            execute(stmt.elseBranch);
        }

        return null;
    }

    @Override
    public Void visitImportStmt(Stmt.Import stmt) {
        Object filePath = evaluate(stmt.path);

        if (!(filePath instanceof String)) {
            return null;
        }

        String basePath = Lox.basePath;
        String pathString = Paths.get(Lox.basePath + (String) filePath).
            normalize().toString();

        // handle circular imports
        if (importedFiles.containsKey(pathString)) {
            return null;
        }
        else {
            importedFiles.put(pathString, true);
        }

        int index = ((String) filePath).lastIndexOf("/");
        fileName = (String) filePath;
        if (index > 0) {
            Lox.basePath = Lox.basePath + ((String) filePath).substring(0, 
                index) + "/";
            fileName = ((String) filePath).substring(index + 1);
        }

        try {
            byte[] bytes = Files.readAllBytes(Paths.get(pathString));
            String source = new String(bytes, Charset.defaultCharset());

            lox.Scanner scanner = new lox.Scanner(source);
            List<Token> tokens = scanner.scanTokens();

            Parser parser = new Parser(tokens);
            parser.currentFile = fileName;
            List<Stmt> statements = parser.parse();
            
            if (Lox.hadError) {
                throw new RuntimeError(stmt.keyword, "Parse error!", 
                    fileName);
            }

            Resolver resolver = new Resolver(this);
            resolver.currentFile = fileName;
            resolver.resolve(statements);

            if (Lox.hadError) {
                throw new RuntimeError(stmt.keyword, "Resolution error!",
                    fileName);
            };
            for (Stmt statement : statements) {
                execute(statement); 
            }

            // reset the path
            Lox.basePath = basePath;
            fileName = "";
        }
        catch (IOException errorException) {
            // reset the path
            Lox.basePath = basePath;
            fileName = "";
            throw new RuntimeError(stmt.keyword,
                "Could not find or load file '" + pathString + "'", fileName);
        }
        return null;
    }

    @Override
    public Void visitPrintStmt(Stmt.Print stmt) {
        Object value = evaluate(stmt.expression);
        System.out.println(stringify(value));
        return null;
    }

    @Override
    public Void visitReturnStmt(Stmt.Return stmt) {
        Object value = null;
        if (stmt.value != null) {
            value = evaluate(stmt.value);
        }

        throw new Exceptions.Return(value);
    }

    @Override
    public Void visitSwitchStmt(Stmt.Switch stmt) {
        Object switchValue = evaluate(stmt.value);

        int startingIndex = -1;
        int defaultIndex = -1;

        for (int i = 0; i < stmt.cases.size(); i++) {
            SwitchCase caseContainer = stmt.cases.get(i);
            
            if (caseContainer.value == null) {
                defaultIndex = i;
            } 
            else {
                Object value = evaluate(caseContainer.value);
                if (isEqual(value, switchValue)) {
                    startingIndex = i;
                    break;
                }
            }
        }
        if (startingIndex == -1) {
            startingIndex = defaultIndex;
        }

        if (startingIndex != -1) {
            try {
                for (int i = startingIndex; i < stmt.cases.size(); i++) {
                    executeBlock(stmt.cases.get(i).statements, environment);
                }
            } catch (Exceptions.Break exceptionBreak) {}
        }
        return null;
    }

    @Override
    public Void visitVarStmt(Stmt.Var stmt) {
        Object value = null;
        if (stmt.initializer != null) {
            value = evaluate(stmt.initializer);
        }

        environment.define(stmt.name.lexeme, value);
        return null;
    }

    @Override
    public Void visitWhileStmt(Stmt.While stmt) {
        try {
            while (isTruthy(evaluate(stmt.condition))) {
                try {
                    execute(stmt.body);
                }
                catch (Exceptions.Continue exceptionContinue){}
            }
        }
        catch(Exceptions.Break exceptionBreak){}

        return null;
    }

    @Override
    public Void visitCommaDeclarationStmt(Stmt.CommaDeclaration stmt) {
        for (Stmt declaration : stmt.declarations) {
            declaration.accept(this);
        }
        return null;
    }

    @Override
    public Object visitAssignExpr(Expr.Assign expr) {
        Object value = evaluate(expr.value);

        Integer distance = locals.get(expr);
        if (distance != null) {
            environment.assignAt(distance, expr.name, value);
        }
        else {
            globals.assign(expr.name, value);
        }
        return value;
    }

    @Override
    public Object visitAssignArrayExpr(Expr.AssignArray expr) {
        Object value = evaluate(expr.value);

        List<Integer> indices = new ArrayList<>();
        for (Expr indexExpr : expr.indices) {
            Object index = evaluate(indexExpr);
            checkNumberOperand(expr.name, index);
            indices.add(((Double)index).intValue());
        }

        Integer distance = locals.get(expr);
        if (distance != null) {
            environment.assignArrayAt(distance, expr.name, indices, value);
        }
        else {
            globals.assignArray(expr.name, indices, value);
        }
        return value;
    }
};
