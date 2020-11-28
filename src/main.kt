import assembler.*;

fun main() {
    println(PasmAssembler("0: nop").parseToAst())
}