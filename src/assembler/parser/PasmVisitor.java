// Generated from /home/joona/realhome/quesler/src/assembler/Pasm.g4 by ANTLR 4.8
package assembler.parser;
import org.antlr.v4.runtime.tree.ParseTreeVisitor;

/**
 * This interface defines a complete generic visitor for a parse tree produced
 * by {@link PasmParser}.
 *
 * @param <T> The return type of the visit operation. Use {@link Void} for
 * operations with no return type.
 */
public interface PasmVisitor<T> extends ParseTreeVisitor<T> {
	/**
	 * Visit a parse tree produced by {@link PasmParser#program}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitProgram(PasmParser.ProgramContext ctx);
	/**
	 * Visit a parse tree produced by {@link PasmParser#block}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitBlock(PasmParser.BlockContext ctx);
	/**
	 * Visit a parse tree produced by {@link PasmParser#codeBlock}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitCodeBlock(PasmParser.CodeBlockContext ctx);
	/**
	 * Visit a parse tree produced by {@link PasmParser#dataBlock}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitDataBlock(PasmParser.DataBlockContext ctx);
	/**
	 * Visit a parse tree produced by {@link PasmParser#stringBlock}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitStringBlock(PasmParser.StringBlockContext ctx);
	/**
	 * Visit a parse tree produced by {@link PasmParser#labelList}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitLabelList(PasmParser.LabelListContext ctx);
	/**
	 * Visit a parse tree produced by {@link PasmParser#codeList}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitCodeList(PasmParser.CodeListContext ctx);
	/**
	 * Visit a parse tree produced by {@link PasmParser#dataList}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitDataList(PasmParser.DataListContext ctx);
	/**
	 * Visit a parse tree produced by {@link PasmParser#stringList}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitStringList(PasmParser.StringListContext ctx);
	/**
	 * Visit a parse tree produced by {@link PasmParser#instruction}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitInstruction(PasmParser.InstructionContext ctx);
	/**
	 * Visit a parse tree produced by {@link PasmParser#argument}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitArgument(PasmParser.ArgumentContext ctx);
	/**
	 * Visit a parse tree produced by {@link PasmParser#label}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitLabel(PasmParser.LabelContext ctx);
	/**
	 * Visit a parse tree produced by {@link PasmParser#data}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitData(PasmParser.DataContext ctx);
	/**
	 * Visit a parse tree produced by {@link PasmParser#stringData}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitStringData(PasmParser.StringDataContext ctx);
	/**
	 * Visit a parse tree produced by {@link PasmParser#maybeIdentifier}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitMaybeIdentifier(PasmParser.MaybeIdentifierContext ctx);
}