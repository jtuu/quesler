import assembler.PasmAssembler

fun main() {
    println(PasmAssembler().parseToAst("0: nop"))
}