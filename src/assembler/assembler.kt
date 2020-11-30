package assembler

import assembler.parser.*
import org.antlr.v4.runtime.CharStreams
import org.antlr.v4.runtime.CommonTokenStream
import org.antlr.v4.runtime.tree.TerminalNode
import java.lang.Exception
import kotlin.math.pow
import opcode_definitions.Opcode

class ParseException(message: String, val sourceLocation: SourceLocation? = null) : Exception(message)

private val INTEGER_MAX = 2.0.pow(31.0).toLong()
private val INTEGER_MIN = -INTEGER_MAX
private const val REAL_MAX = Float.MAX_VALUE
private const val REAL_MIN = Float.MIN_VALUE

operator fun Number.compareTo(other: Number) = this.toDouble().compareTo(other.toDouble())

private fun splitKeep(string: String, delimiter: String): List<String> {
    return string.split(delimiter).map{it + delimiter}
}

class PasmAssembler {
    var warningsAreErrors: Boolean = false

    private fun preprocessSourceCode(sourceCode: String): String {
        return sourceCode + "\n"
    }

    fun parseToAst(sourceCode: String): ProgramNode {
        val visitor = PasmVisitor(preprocessSourceCode(sourceCode))
        val lexer = PasmLexer(CharStreams.fromString(preprocessSourceCode(sourceCode)))
        val parser = PasmParser(CommonTokenStream(lexer))
        val parseTree = parser.program()
        visitor.warningsAreErrors = warningsAreErrors
        return visitor.visitProgram(parseTree)
    }

    fun assembleAst(program: ProgramNode): ByteArray {
        return ByteArray(0) // TODO
    }

    fun assemble(sourceCode: String): ByteArray {
        return assembleAst(parseToAst(sourceCode))
    }
}

private class PasmVisitor(sourceCode: String) : PasmBaseVisitor<AstNode>() {
    var warningsAreErrors: Boolean = false

    private val sourceLines: List<String> = splitKeep(sourceCode, "\n")

    private fun sourceCodeSnippet(sourceLocation: SourceLocation): String {
        val line = sourceLines.getOrNull(sourceLocation.row) ?: return ""

        if (sourceLocation.length == null) {
            return line
        }

        return line.substring(sourceLocation.column, sourceLocation.column + sourceLocation.length)
    }

    private fun warning(message: String, sourceLocation: SourceLocation?) {
        var messageWithLocation = "$message $sourceLocation"

        if (sourceLocation != null) {
            messageWithLocation += " (${sourceCodeSnippet(sourceLocation)})"
        }

        if (warningsAreErrors) {
            throw ParseException(messageWithLocation, sourceLocation)
        }

        System.err.println(messageWithLocation)
    }

    private fun <A : Number>ensureValueFits(value: A, min: A, max: A, sourceLocation: SourceLocation) {
        if (value > max) {
            warning("Value $value too large (max $max)", sourceLocation)
        } else if (value < min) {
            warning("Value $value too small (min $min)", sourceLocation)
        }
    }

    fun parseString(token: TerminalNode): StringNode {
        return StringNode(token.text.trim('"', '\''), SourceLocation.fromToken(token.symbol))
    }

    override fun visitArgument(ctx: PasmParser.ArgumentContext): ArgumentNode<*> {
        return when (ctx.value.type) {
            PasmLexer.Integer -> {
                val asInt = ctx.Integer()
                val value = asInt.text.toLong()
                val srcLoc = SourceLocation.fromToken(asInt.symbol)
                ensureValueFits(value, INTEGER_MIN, INTEGER_MAX, srcLoc)
                IntegerNode(value, srcLoc)
            }
            PasmLexer.Real -> {
                val asReal = ctx.Real()
                val value = asReal.text.toDouble()
                val srcLoc = SourceLocation.fromToken(asReal.symbol)
                ensureValueFits(value, REAL_MIN, REAL_MAX, srcLoc)
                RealNode(value, srcLoc)
            }
            PasmLexer.HexNumber -> {
                val asHex = ctx.HexNumber()
                val value = asHex.text.substring(2).toLong(radix = 16)
                val srcLoc = SourceLocation.fromToken(asHex.symbol)
                ensureValueFits(value, INTEGER_MIN, INTEGER_MAX, srcLoc)
                IntegerNode(value, SourceLocation.fromToken(asHex.symbol))
            }
            PasmLexer.String -> {
                val asString = ctx.String()
                parseString(asString)
            }
            PasmLexer.Register -> {
                val asRegister = ctx.Register()
                RegisterNode(asRegister.text.toLong(), SourceLocation.fromToken(asRegister.symbol))
            }
            else -> throw ParseException("Malformed argument", SourceLocation.fromToken(ctx.start))
        }
    }

    override fun visitInstruction(ctx: PasmParser.InstructionContext): InstructionNode {
        val identifier = ctx.opcode.Identifier()
        val opcodeMnemonic = identifier.text
        val opcodeSrcLoc = SourceLocation.fromToken(identifier.symbol)

        val opcode = when (opcodeMnemonic.toLowerCase()) {
            "nop" -> Opcode.NOP
            else -> throw ParseException("Unknown opcode $opcodeMnemonic", opcodeSrcLoc)
        }

        val arguments = ctx.arguments.map{visitArgument(it)}

        return InstructionNode(OpcodeNode(opcode, opcodeSrcLoc), arguments, SourceLocation.fromToken(ctx.start))
    }

    override fun visitLabel(ctx: PasmParser.LabelContext): LabelNode {
        return LabelNode(ctx.Integer().text.toLong(), SourceLocation.fromToken(ctx.start))
    }

    override fun visitCodeBlock(ctx: PasmParser.CodeBlockContext): CodeBlockNode {
        return CodeBlockNode(
                ctx.labelList().label().map{visitLabel(it)},
                ctx.codeList().instruction().map{visitInstruction(it)},
                SourceLocation.fromToken(ctx.start))
    }

    override fun visitDataBlock(ctx: PasmParser.DataBlockContext): DataBlockNode {
        val bytes = ctx.dataList().data().flatMap{it.HexDigitPair().map{it.text.toByte(radix = 16)}}
        return DataBlockNode(
                ctx.labelList().label().map{visitLabel(it)},
                ByteArray(bytes.size) {i -> bytes[i]},
                SourceLocation.fromToken(ctx.start))
    }

    override fun visitStringBlock(ctx: PasmParser.StringBlockContext): StringDataBlockNode {
        return StringDataBlockNode(
                ctx.labelList().label().map{visitLabel(it)},
                ctx.stringList().stringData().map{parseString(it.String())},
                SourceLocation.fromToken(ctx.start))
    }

    override fun visitBlock(ctx: PasmParser.BlockContext): BlockNode {
        val asCodeBlock = ctx.codeBlock()
        if (asCodeBlock != null) {
            return visitCodeBlock(asCodeBlock)
        }

        val asDataBlock = ctx.dataBlock()
        if (asDataBlock != null) {
            return visitDataBlock(asDataBlock)
        }

        val asStringBlock = ctx.stringBlock()
        if (asStringBlock != null) {
            return visitStringBlock(asStringBlock)
        }

        throw ParseException("Malformed block", SourceLocation.fromToken(ctx.start))
    }

    override fun visitProgram(ctx: PasmParser.ProgramContext): ProgramNode {
        return ProgramNode(ctx.blocks.map{visitBlock(it)}, SourceLocation.fromToken(ctx.start))
    }
}