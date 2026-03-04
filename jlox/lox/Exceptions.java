package lox;

class Exceptions extends RuntimeException {
    Exceptions() {
        super(null, null, false, false);
    }

    static class Break extends Exceptions {    
        Break() {}
    }

    static class Continue extends Exceptions {
        Continue() {}
    }

    static class Return extends Exceptions {
        final Object value;

        Return(Object value) {
            this.value = value;
        }
    }
}
