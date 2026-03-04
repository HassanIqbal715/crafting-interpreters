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
    
    Object getArray(Token name, List<Integer> indices) {
        if (values.containsKey(name.lexeme)) {
            Object value = values.get(name.lexeme);

            if (value == null) 
                throw new RuntimeError(name, 
                    "Uninitialized array '"  + name.lexeme + "'.");
            if (!(value instanceof List)) {
                throw new RuntimeError(name, 
                    "'" + name.lexeme + "' is not an array");
            }
            return getArrayValue(name, value, indices, 0);
        }

        if (enclosing != null) return enclosing.getArray(name, indices);

        throw new RuntimeError(name, "Undefined array '" + name.lexeme + "'.");
    }

    Object getArrayValue(Token name, Object elements, List<Integer> indices, 
        int i) {
        int index = indices.get(i);

        if (index < 0 || index >= ((List<?>)elements).size()) {
            throw new RuntimeError(name, 
                "Index " + index + " is out of bounds of array '");
        }

        if (i >= indices.size() - 1) {
            return (Object) ((List<?>) elements).get(index);
        }
        
        checkList(name, elements);
        return getArrayValue(name, ((List<?>) elements).get(index), indices, 
            i + 1);
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
    void assignArray(Token name, List<Integer> indices, Object value) {
        if (values.containsKey(name.lexeme)) {
            Object list = values.get(name.lexeme);
            List<Object> elements = (List<Object>) list;
            serArrayValue(name, elements, indices, value, 0);
            return;
        }

        if (enclosing != null) {
            enclosing.assignArray(name, indices, value);
            return;
        }
        
        throw new RuntimeError(name, "Undefined array '" + name.lexeme + "'.");
    }

    @SuppressWarnings("unchecked")
    void serArrayValue(Token name, Object elements, List<Integer> indices, 
        Object value, int i) {

        int index = indices.get(i);
        if (index < 0 || index >= ((List<Object>)elements).size()) {
            throw new RuntimeError(name, 
                "Index " + index + " is out of bounds of array '");
        }

        if (i >= indices.size() - 1) {
            ((List<Object>)elements).set(index, value);
            return;
        }

        checkList(name, elements);
        serArrayValue(name, ((List<Object>) elements).get(index), indices, 
            value, i + 1);
    }

    void checkList(Token name, Object list) {
        if (list instanceof List) return;
        throw new RuntimeError(name, "Expect type elements.");
    }

    void define(String name, Object value) {
        values.put(name, value);
    }

    Environment ancestor(int distance) {
        Environment environment = this;
        for (int i = 0; i < distance; i++) {
            environment = environment.enclosing;
        }

        return environment;
    }

    Object getArrayAt(int distance, Token name, List<Integer> indices) {
        return ancestor(distance).getArray(name, indices);
    }

    Object getAt(int distance, String name) {
        return ancestor(distance).values.get(name);
    }

    void assignArrayAt(int distance, Token name, List<Integer> indices, 
            Object value) {
        ancestor(distance).assignArray(name, indices, value);
    }

    void assignAt(int distance, Token name, Object value) {
        ancestor(distance).values.put(name.lexeme, value);
    }
}
