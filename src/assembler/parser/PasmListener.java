// Generated from /home/joona/realhome/quesler/src/assembler/Pasm.g4 by ANTLR 4.8
package assembler.parser;
import org.antlr.v4.runtime.tree.ParseTreeListener;

/**
 * This interface defines a complete listener for a parse tree produced by
 * {@link PasmParser}.
 */
public interface PasmListener extends ParseTreeListener {
	/**
	 * Enter a parse tree produced by {@link PasmParser#program}.
	 * @param ctx the parse tree
	 */
	void enterProgram(PasmParser.ProgramContext ctx);
	/**
	 * Exit a parse tree produced by {@link PasmParser#program}.
	 * @param ctx the parse tree
	 */
	void exitProgram(PasmParser.ProgramContext ctx);
	/**
	 * Enter a parse tree produced by {@link PasmParser#block}.
	 * @param ctx the parse tree
	 */
	void enterBlock(PasmParser.BlockContext ctx);
	/**
	 * Exit a parse tree produced by {@link PasmParser#block}.
	 * @param ctx the parse tree
	 */
	void exitBlock(PasmParser.BlockContext ctx);
	/**
	 * Enter a parse tree produced by {@link PasmParser#codeBlock}.
	 * @param ctx the parse tree
	 */
	void enterCodeBlock(PasmParser.CodeBlockContext ctx);
	/**
	 * Exit a parse tree produced by {@link PasmParser#codeBlock}.
	 * @param ctx the parse tree
	 */
	void exitCodeBlock(PasmParser.CodeBlockContext ctx);
	/**
	 * Enter a parse tree produced by {@link PasmParser#dataBlock}.
	 * @param ctx the parse tree
	 */
	void enterDataBlock(PasmParser.DataBlockContext ctx);
	/**
	 * Exit a parse tree produced by {@link PasmParser#dataBlock}.
	 * @param ctx the parse tree
	 */
	void exitDataBlock(PasmParser.DataBlockContext ctx);
	/**
	 * Enter a parse tree produced by {@link PasmParser#stringBlock}.
	 * @param ctx the parse tree
	 */
	void enterStringBlock(PasmParser.StringBlockContext ctx);
	/**
	 * Exit a parse tree produced by {@link PasmParser#stringBlock}.
	 * @param ctx the parse tree
	 */
	void exitStringBlock(PasmParser.StringBlockContext ctx);
	/**
	 * Enter a parse tree produced by {@link PasmParser#labelList}.
	 * @param ctx the parse tree
	 */
	void enterLabelList(PasmParser.LabelListContext ctx);
	/**
	 * Exit a parse tree produced by {@link PasmParser#labelList}.
	 * @param ctx the parse tree
	 */
	void exitLabelList(PasmParser.LabelListContext ctx);
	/**
	 * Enter a parse tree produced by {@link PasmParser#codeList}.
	 * @param ctx the parse tree
	 */
	void enterCodeList(PasmParser.CodeListContext ctx);
	/**
	 * Exit a parse tree produced by {@link PasmParser#codeList}.
	 * @param ctx the parse tree
	 */
	void exitCodeList(PasmParser.CodeListContext ctx);
	/**
	 * Enter a parse tree produced by {@link PasmParser#dataList}.
	 * @param ctx the parse tree
	 */
	void enterDataList(PasmParser.DataListContext ctx);
	/**
	 * Exit a parse tree produced by {@link PasmParser#dataList}.
	 * @param ctx the parse tree
	 */
	void exitDataList(PasmParser.DataListContext ctx);
	/**
	 * Enter a parse tree produced by {@link PasmParser#stringList}.
	 * @param ctx the parse tree
	 */
	void enterStringList(PasmParser.StringListContext ctx);
	/**
	 * Exit a parse tree produced by {@link PasmParser#stringList}.
	 * @param ctx the parse tree
	 */
	void exitStringList(PasmParser.StringListContext ctx);
	/**
	 * Enter a parse tree produced by {@link PasmParser#instruction}.
	 * @param ctx the parse tree
	 */
	void enterInstruction(PasmParser.InstructionContext ctx);
	/**
	 * Exit a parse tree produced by {@link PasmParser#instruction}.
	 * @param ctx the parse tree
	 */
	void exitInstruction(PasmParser.InstructionContext ctx);
	/**
	 * Enter a parse tree produced by {@link PasmParser#argument}.
	 * @param ctx the parse tree
	 */
	void enterArgument(PasmParser.ArgumentContext ctx);
	/**
	 * Exit a parse tree produced by {@link PasmParser#argument}.
	 * @param ctx the parse tree
	 */
	void exitArgument(PasmParser.ArgumentContext ctx);
	/**
	 * Enter a parse tree produced by {@link PasmParser#label}.
	 * @param ctx the parse tree
	 */
	void enterLabel(PasmParser.LabelContext ctx);
	/**
	 * Exit a parse tree produced by {@link PasmParser#label}.
	 * @param ctx the parse tree
	 */
	void exitLabel(PasmParser.LabelContext ctx);
	/**
	 * Enter a parse tree produced by {@link PasmParser#data}.
	 * @param ctx the parse tree
	 */
	void enterData(PasmParser.DataContext ctx);
	/**
	 * Exit a parse tree produced by {@link PasmParser#data}.
	 * @param ctx the parse tree
	 */
	void exitData(PasmParser.DataContext ctx);
	/**
	 * Enter a parse tree produced by {@link PasmParser#stringData}.
	 * @param ctx the parse tree
	 */
	void enterStringData(PasmParser.StringDataContext ctx);
	/**
	 * Exit a parse tree produced by {@link PasmParser#stringData}.
	 * @param ctx the parse tree
	 */
	void exitStringData(PasmParser.StringDataContext ctx);
	/**
	 * Enter a parse tree produced by {@link PasmParser#maybeIdentifier}.
	 * @param ctx the parse tree
	 */
	void enterMaybeIdentifier(PasmParser.MaybeIdentifierContext ctx);
	/**
	 * Exit a parse tree produced by {@link PasmParser#maybeIdentifier}.
	 * @param ctx the parse tree
	 */
	void exitMaybeIdentifier(PasmParser.MaybeIdentifierContext ctx);
}