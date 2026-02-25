package lox;

import java.util.HashMap;
import java.util.Map;
import java.util.List;

class Environment {
    final Environment enclosing;
    private final Map<String, Object> values = new HashMap<>();

    Environment() {
        enclosing = null;
    }

    Environment(Environment enclosing) {
        this.enclosing = enclosing;
    }

    Object get(Token name) {
        if (values.containsKey(name.lexeme)) {
            if (values.get(name.lexeme) == null) 
                throw new RuntimeError(name, 
                    "Uninitialized variable '"  + name.lexeme + "'.");

            return values.get(name.lexeme);
        }

        if (enclosing != null) return enclosing.get(name);

        throw new RuntimeError(name, 
            "Undefined variable '" + name.lexeme + "'.");
    }
    
    Object getArray(Token name, int index) {
        if (values.containsKey(name.lexeme)) {
            Object value = values.get(name.lexeme);

            if (value == null) 
                throw new RuntimeError(name, 
                    "Uninitialized array '"  + name.lexeme + "'.");
            if (!(value instanceof List)) {
                throw new RuntimeError(name, 
                    "'" + name.lexeme + "' is not an array");
            }

            // For type safety
            List<?> rawElements = (List<?>) value;
            
            if (index < 0 || index >= rawElements.size()) {
                throw new RuntimeError(name, 
                    "Index " + index + " is out of bounds of array '");
            }

            return (Object) rawElements.get(index);
        }

        if (enclosing != null) return enclosing.getArray(name, index);

        throw new RuntimeError(name, "Undefined array '" + name.lexeme + "'.");
    }

    void assign(Token name, Object value) {
        if (values.containsKey(name.lexeme)) {
            values.put(name.lexeme, value);
            return;
        }
        
        if (enclosing != null) {
            enclosing.assign(name, value);
            return;
        }

        throw new RuntimeError(name,
            "Undefined variable'" + name.lexeme + "'.");
    }

    @SuppressWarnings("unchecked")
    void assignArray(Token name, int index, Object value) {
        if (values.containsKey(name.lexeme)) {
            Object list = values.get(name.lexeme);
            List<Object> elements = (List<Object>) list;
            
            if (index < 0 || index >= elements.size()) {
                throw new RuntimeError(name, 
                    "Index " + index + " is out of bounds of array '");
            }

            elements.set(index, value);
            return;
        }

        if (enclosing != null) {
            enclosing.assignArray(name, index, value);
            return;
        }
        
        throw new RuntimeError(name, "Undefined array '" + name.lexeme + "'.");
    }

    void define(String name, Object value) {
        values.put(name, value);
    }
}
