import assembler.*;

fun main() {
    println(PasmAssembler().parseToAst("0: nop"))
}