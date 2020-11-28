package assembler

import org.antlr.v4.runtime.Token

enum class Opcode {
    NOP
}

class SourceLocation(val row: Int, val column: Int, val length: Int?) {
    companion object {
        fun fromToken(token: Token): SourceLocation {
            return SourceLocation(token.line, token.charPositionInLine, token.stopIndex - token.startIndex)
        }
    }

    override fun toString(): String {
        return "($row:$column)"
    }
}

abstract class AstNode(val sourceLocation: SourceLocation?) {
    abstract override fun toString(): String
}

class ProgramNode(val blocks: List<BlockNode>, sourceLocation: SourceLocation?) : AstNode(sourceLocation) {
    override fun toString(): String {
        var str = ""

        for (block in blocks) {
            str += block.toString()
        }

        return str
    }
}

class LabelNode(val value: Long, sourceLocation: SourceLocation?) : AstNode(sourceLocation) {
    override fun toString(): String {
        return "$value:"
    }
}

class OpcodeNode(val opcode: Opcode, sourceLocation: SourceLocation?) : AstNode(sourceLocation) {
    override fun toString(): String {
        return when (opcode) {
            Opcode.NOP -> "nop"
        }
    }
}

abstract class ArgumentNode<T>(val value: T, sourceLocation: SourceLocation?) : AstNode(sourceLocation)

class IntegerNode(value: Long, sourceLocation: SourceLocation?) : ArgumentNode<Long>(value, sourceLocation) {
    override fun toString(): String {
        return value.toString()
    }
}

class RealNode(value: Double, sourceLocation: SourceLocation?) : ArgumentNode<Double>(value, sourceLocation) {
    override fun toString(): String {
        return value.toString()
    }
}

class StringNode(value: String, sourceLocation: SourceLocation?) : ArgumentNode<String>(value, sourceLocation) {
    override fun toString(): String {
        return "\"$value\""
    }
}

class RegisterNode(value: Long, sourceLocation: SourceLocation?) : ArgumentNode<Long>(value, sourceLocation) {
    override fun toString(): String {
        return "R$value"
    }
}

class InstructionNode(val opcode: OpcodeNode, val arguments: List<ArgumentNode<*>>, sourceLocation: SourceLocation?) : AstNode(sourceLocation) {
    override fun toString(): String {
        var str = opcode.toString()

        if (arguments.isEmpty()) {
            return str
        }

        str += " ${arguments[0]}"
        for (i in 1 until arguments.size) {
            str += ", ${arguments[i]}"
        }

        return str
    }
}

abstract class BlockNode(val labels: List<LabelNode>, sourceLocation: SourceLocation?) : AstNode(sourceLocation) {
    companion object {
        var Indentation = 4
        set(value) {
            indentation = " ".repeat(value)
            field = value
        }

        @JvmStatic protected var indentation = " ".repeat(Indentation)
    }

    protected fun labelsToString(): String {
        var str = ""

        for (label in labels) {
            str += "$label\n"
        }

        return str
    }
}

class CodeBlockNode(labels: List<LabelNode>, val instructions: List<InstructionNode>, sourceLocation: SourceLocation?) : BlockNode(labels, sourceLocation) {
    override fun toString(): String {
        var str = labelsToString()

        for (instr in instructions) {
            str += "$indentation$instr\n"
        }

        return str
    }
}

class DataBlockNode(labels: List<LabelNode>, val bytes: ByteArray, sourceLocation: SourceLocation?) : BlockNode(labels, sourceLocation) {
    companion object {
        var BytesPerLine = 8
    }

    override fun toString(): String {
        var str = labelsToString()

        for (i in bytes.indices step BytesPerLine) {
            str += "${indentation}HEX:"

            for (j in i .. i + BytesPerLine) {
                str += " " + bytes[j].toString(radix = 16)
            }
        }

        return str
    }
}

class StringDataBlockNode(labels: List<LabelNode>, val strings: List<StringNode>, sourceLocation: SourceLocation?) : BlockNode(labels, sourceLocation) {
    override fun toString(): String {
        var str = labelsToString()

        for (s in strings) {
            str += "${indentation}STR: s\n"
        }

        return str
    }
}