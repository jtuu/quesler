// Generated from /home/joona/realhome/quesler/src/assembler/Pasm.g4 by ANTLR 4.8
package assembler.parser;
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.misc.*;
import org.antlr.v4.runtime.tree.*;
import java.util.List;
import java.util.Iterator;
import java.util.ArrayList;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class PasmParser extends Parser {
	static { RuntimeMetaData.checkVersion("4.8", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		Register=1, Integer=2, Real=3, HexNumber=4, HexDigitPair=5, LabelSuffix=6, 
		DataPrefix=7, StringDataPrefix=8, Identifier=9, String=10, SingleQuoteString=11, 
		DoubleQuoteString=12, StatementSeparator=13, EndOfLine=14, ArgumentSeparator=15;
	public static final int
		RULE_program = 0, RULE_block = 1, RULE_codeBlock = 2, RULE_dataBlock = 3, 
		RULE_stringBlock = 4, RULE_labelList = 5, RULE_codeList = 6, RULE_dataList = 7, 
		RULE_stringList = 8, RULE_instruction = 9, RULE_argument = 10, RULE_label = 11, 
		RULE_data = 12, RULE_stringData = 13, RULE_maybeIdentifier = 14;
	private static String[] makeRuleNames() {
		return new String[] {
			"program", "block", "codeBlock", "dataBlock", "stringBlock", "labelList", 
			"codeList", "dataList", "stringList", "instruction", "argument", "label", 
			"data", "stringData", "maybeIdentifier"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, null, null, null, null, null, "':'", "'HEX:'", "'STR:'"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, "Register", "Integer", "Real", "HexNumber", "HexDigitPair", "LabelSuffix", 
			"DataPrefix", "StringDataPrefix", "Identifier", "String", "SingleQuoteString", 
			"DoubleQuoteString", "StatementSeparator", "EndOfLine", "ArgumentSeparator"
		};
	}
	private static final String[] _SYMBOLIC_NAMES = makeSymbolicNames();
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}

	@Override
	public String getGrammarFileName() { return "Pasm.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public ATN getATN() { return _ATN; }

	public PasmParser(TokenStream input) {
		super(input);
		_interp = new ParserATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	public static class ProgramContext extends ParserRuleContext {
		public BlockContext block;
		public List<BlockContext> blocks = new ArrayList<BlockContext>();
		public TerminalNode EOF() { return getToken(PasmParser.EOF, 0); }
		public List<BlockContext> block() {
			return getRuleContexts(BlockContext.class);
		}
		public BlockContext block(int i) {
			return getRuleContext(BlockContext.class,i);
		}
		public ProgramContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_program; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof PasmListener ) ((PasmListener)listener).enterProgram(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof PasmListener ) ((PasmListener)listener).exitProgram(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof PasmVisitor ) return ((PasmVisitor<? extends T>)visitor).visitProgram(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ProgramContext program() throws RecognitionException {
		ProgramContext _localctx = new ProgramContext(_ctx, getState());
		enterRule(_localctx, 0, RULE_program);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(33);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==Integer) {
				{
				{
				setState(30);
				((ProgramContext)_localctx).block = block();
				((ProgramContext)_localctx).blocks.add(((ProgramContext)_localctx).block);
				}
				}
				setState(35);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(36);
			match(EOF);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class BlockContext extends ParserRuleContext {
		public CodeBlockContext codeBlock() {
			return getRuleContext(CodeBlockContext.class,0);
		}
		public DataBlockContext dataBlock() {
			return getRuleContext(DataBlockContext.class,0);
		}
		public StringBlockContext stringBlock() {
			return getRuleContext(StringBlockContext.class,0);
		}
		public BlockContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_block; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof PasmListener ) ((PasmListener)listener).enterBlock(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof PasmListener ) ((PasmListener)listener).exitBlock(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof PasmVisitor ) return ((PasmVisitor<? extends T>)visitor).visitBlock(this);
			else return visitor.visitChildren(this);
		}
	}

	public final BlockContext block() throws RecognitionException {
		BlockContext _localctx = new BlockContext(_ctx, getState());
		enterRule(_localctx, 2, RULE_block);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(41);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,1,_ctx) ) {
			case 1:
				{
				setState(38);
				codeBlock();
				}
				break;
			case 2:
				{
				setState(39);
				dataBlock();
				}
				break;
			case 3:
				{
				setState(40);
				stringBlock();
				}
				break;
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class CodeBlockContext extends ParserRuleContext {
		public LabelListContext labelList() {
			return getRuleContext(LabelListContext.class,0);
		}
		public CodeListContext codeList() {
			return getRuleContext(CodeListContext.class,0);
		}
		public CodeBlockContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_codeBlock; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof PasmListener ) ((PasmListener)listener).enterCodeBlock(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof PasmListener ) ((PasmListener)listener).exitCodeBlock(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof PasmVisitor ) return ((PasmVisitor<? extends T>)visitor).visitCodeBlock(this);
			else return visitor.visitChildren(this);
		}
	}

	public final CodeBlockContext codeBlock() throws RecognitionException {
		CodeBlockContext _localctx = new CodeBlockContext(_ctx, getState());
		enterRule(_localctx, 4, RULE_codeBlock);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(43);
			labelList();
			setState(44);
			codeList();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class DataBlockContext extends ParserRuleContext {
		public LabelListContext labelList() {
			return getRuleContext(LabelListContext.class,0);
		}
		public DataListContext dataList() {
			return getRuleContext(DataListContext.class,0);
		}
		public DataBlockContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_dataBlock; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof PasmListener ) ((PasmListener)listener).enterDataBlock(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof PasmListener ) ((PasmListener)listener).exitDataBlock(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof PasmVisitor ) return ((PasmVisitor<? extends T>)visitor).visitDataBlock(this);
			else return visitor.visitChildren(this);
		}
	}

	public final DataBlockContext dataBlock() throws RecognitionException {
		DataBlockContext _localctx = new DataBlockContext(_ctx, getState());
		enterRule(_localctx, 6, RULE_dataBlock);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(46);
			labelList();
			setState(47);
			dataList();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class StringBlockContext extends ParserRuleContext {
		public LabelListContext labelList() {
			return getRuleContext(LabelListContext.class,0);
		}
		public StringListContext stringList() {
			return getRuleContext(StringListContext.class,0);
		}
		public StringBlockContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_stringBlock; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof PasmListener ) ((PasmListener)listener).enterStringBlock(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof PasmListener ) ((PasmListener)listener).exitStringBlock(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof PasmVisitor ) return ((PasmVisitor<? extends T>)visitor).visitStringBlock(this);
			else return visitor.visitChildren(this);
		}
	}

	public final StringBlockContext stringBlock() throws RecognitionException {
		StringBlockContext _localctx = new StringBlockContext(_ctx, getState());
		enterRule(_localctx, 8, RULE_stringBlock);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(49);
			labelList();
			setState(50);
			stringList();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class LabelListContext extends ParserRuleContext {
		public List<LabelContext> label() {
			return getRuleContexts(LabelContext.class);
		}
		public LabelContext label(int i) {
			return getRuleContext(LabelContext.class,i);
		}
		public List<TerminalNode> EndOfLine() { return getTokens(PasmParser.EndOfLine); }
		public TerminalNode EndOfLine(int i) {
			return getToken(PasmParser.EndOfLine, i);
		}
		public List<TerminalNode> StatementSeparator() { return getTokens(PasmParser.StatementSeparator); }
		public TerminalNode StatementSeparator(int i) {
			return getToken(PasmParser.StatementSeparator, i);
		}
		public LabelListContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_labelList; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof PasmListener ) ((PasmListener)listener).enterLabelList(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof PasmListener ) ((PasmListener)listener).exitLabelList(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof PasmVisitor ) return ((PasmVisitor<? extends T>)visitor).visitLabelList(this);
			else return visitor.visitChildren(this);
		}
	}

	public final LabelListContext labelList() throws RecognitionException {
		LabelListContext _localctx = new LabelListContext(_ctx, getState());
		enterRule(_localctx, 10, RULE_labelList);
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(57);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,2,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					{
					{
					setState(52);
					label();
					setState(53);
					match(EndOfLine);
					}
					} 
				}
				setState(59);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,2,_ctx);
			}
			setState(60);
			label();
			setState(64);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==StatementSeparator || _la==EndOfLine) {
				{
				{
				setState(61);
				_la = _input.LA(1);
				if ( !(_la==StatementSeparator || _la==EndOfLine) ) {
				_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				}
				}
				setState(66);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class CodeListContext extends ParserRuleContext {
		public List<InstructionContext> instruction() {
			return getRuleContexts(InstructionContext.class);
		}
		public InstructionContext instruction(int i) {
			return getRuleContext(InstructionContext.class,i);
		}
		public List<TerminalNode> StatementSeparator() { return getTokens(PasmParser.StatementSeparator); }
		public TerminalNode StatementSeparator(int i) {
			return getToken(PasmParser.StatementSeparator, i);
		}
		public List<TerminalNode> EndOfLine() { return getTokens(PasmParser.EndOfLine); }
		public TerminalNode EndOfLine(int i) {
			return getToken(PasmParser.EndOfLine, i);
		}
		public CodeListContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_codeList; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof PasmListener ) ((PasmListener)listener).enterCodeList(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof PasmListener ) ((PasmListener)listener).exitCodeList(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof PasmVisitor ) return ((PasmVisitor<? extends T>)visitor).visitCodeList(this);
			else return visitor.visitChildren(this);
		}
	}

	public final CodeListContext codeList() throws RecognitionException {
		CodeListContext _localctx = new CodeListContext(_ctx, getState());
		enterRule(_localctx, 12, RULE_codeList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(74); 
			_errHandler.sync(this);
			_la = _input.LA(1);
			do {
				{
				{
				setState(67);
				instruction();
				setState(71);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while (_la==StatementSeparator || _la==EndOfLine) {
					{
					{
					setState(68);
					_la = _input.LA(1);
					if ( !(_la==StatementSeparator || _la==EndOfLine) ) {
					_errHandler.recoverInline(this);
					}
					else {
						if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
						_errHandler.reportMatch(this);
						consume();
					}
					}
					}
					setState(73);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				}
				}
				setState(76); 
				_errHandler.sync(this);
				_la = _input.LA(1);
			} while ( (((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << Register) | (1L << HexDigitPair) | (1L << Identifier))) != 0) );
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class DataListContext extends ParserRuleContext {
		public List<DataContext> data() {
			return getRuleContexts(DataContext.class);
		}
		public DataContext data(int i) {
			return getRuleContext(DataContext.class,i);
		}
		public List<TerminalNode> StatementSeparator() { return getTokens(PasmParser.StatementSeparator); }
		public TerminalNode StatementSeparator(int i) {
			return getToken(PasmParser.StatementSeparator, i);
		}
		public List<TerminalNode> EndOfLine() { return getTokens(PasmParser.EndOfLine); }
		public TerminalNode EndOfLine(int i) {
			return getToken(PasmParser.EndOfLine, i);
		}
		public DataListContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_dataList; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof PasmListener ) ((PasmListener)listener).enterDataList(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof PasmListener ) ((PasmListener)listener).exitDataList(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof PasmVisitor ) return ((PasmVisitor<? extends T>)visitor).visitDataList(this);
			else return visitor.visitChildren(this);
		}
	}

	public final DataListContext dataList() throws RecognitionException {
		DataListContext _localctx = new DataListContext(_ctx, getState());
		enterRule(_localctx, 14, RULE_dataList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(85); 
			_errHandler.sync(this);
			_la = _input.LA(1);
			do {
				{
				{
				setState(78);
				data();
				setState(82);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while (_la==StatementSeparator || _la==EndOfLine) {
					{
					{
					setState(79);
					_la = _input.LA(1);
					if ( !(_la==StatementSeparator || _la==EndOfLine) ) {
					_errHandler.recoverInline(this);
					}
					else {
						if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
						_errHandler.reportMatch(this);
						consume();
					}
					}
					}
					setState(84);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				}
				}
				setState(87); 
				_errHandler.sync(this);
				_la = _input.LA(1);
			} while ( _la==DataPrefix );
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class StringListContext extends ParserRuleContext {
		public List<StringDataContext> stringData() {
			return getRuleContexts(StringDataContext.class);
		}
		public StringDataContext stringData(int i) {
			return getRuleContext(StringDataContext.class,i);
		}
		public List<TerminalNode> StatementSeparator() { return getTokens(PasmParser.StatementSeparator); }
		public TerminalNode StatementSeparator(int i) {
			return getToken(PasmParser.StatementSeparator, i);
		}
		public List<TerminalNode> EndOfLine() { return getTokens(PasmParser.EndOfLine); }
		public TerminalNode EndOfLine(int i) {
			return getToken(PasmParser.EndOfLine, i);
		}
		public StringListContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_stringList; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof PasmListener ) ((PasmListener)listener).enterStringList(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof PasmListener ) ((PasmListener)listener).exitStringList(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof PasmVisitor ) return ((PasmVisitor<? extends T>)visitor).visitStringList(this);
			else return visitor.visitChildren(this);
		}
	}

	public final StringListContext stringList() throws RecognitionException {
		StringListContext _localctx = new StringListContext(_ctx, getState());
		enterRule(_localctx, 16, RULE_stringList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(96); 
			_errHandler.sync(this);
			_la = _input.LA(1);
			do {
				{
				{
				setState(89);
				stringData();
				setState(93);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while (_la==StatementSeparator || _la==EndOfLine) {
					{
					{
					setState(90);
					_la = _input.LA(1);
					if ( !(_la==StatementSeparator || _la==EndOfLine) ) {
					_errHandler.recoverInline(this);
					}
					else {
						if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
						_errHandler.reportMatch(this);
						consume();
					}
					}
					}
					setState(95);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				}
				}
				setState(98); 
				_errHandler.sync(this);
				_la = _input.LA(1);
			} while ( _la==StringDataPrefix );
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class InstructionContext extends ParserRuleContext {
		public MaybeIdentifierContext opcode;
		public ArgumentContext argument;
		public List<ArgumentContext> arguments = new ArrayList<ArgumentContext>();
		public TerminalNode EndOfLine() { return getToken(PasmParser.EndOfLine, 0); }
		public MaybeIdentifierContext maybeIdentifier() {
			return getRuleContext(MaybeIdentifierContext.class,0);
		}
		public List<TerminalNode> StatementSeparator() { return getTokens(PasmParser.StatementSeparator); }
		public TerminalNode StatementSeparator(int i) {
			return getToken(PasmParser.StatementSeparator, i);
		}
		public List<ArgumentContext> argument() {
			return getRuleContexts(ArgumentContext.class);
		}
		public ArgumentContext argument(int i) {
			return getRuleContext(ArgumentContext.class,i);
		}
		public List<TerminalNode> ArgumentSeparator() { return getTokens(PasmParser.ArgumentSeparator); }
		public TerminalNode ArgumentSeparator(int i) {
			return getToken(PasmParser.ArgumentSeparator, i);
		}
		public InstructionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_instruction; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof PasmListener ) ((PasmListener)listener).enterInstruction(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof PasmListener ) ((PasmListener)listener).exitInstruction(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof PasmVisitor ) return ((PasmVisitor<? extends T>)visitor).visitInstruction(this);
			else return visitor.visitChildren(this);
		}
	}

	public final InstructionContext instruction() throws RecognitionException {
		InstructionContext _localctx = new InstructionContext(_ctx, getState());
		enterRule(_localctx, 18, RULE_instruction);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(100);
			((InstructionContext)_localctx).opcode = maybeIdentifier();
			setState(110);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,11,_ctx) ) {
			case 1:
				{
				setState(101);
				match(StatementSeparator);
				setState(102);
				((InstructionContext)_localctx).argument = argument();
				((InstructionContext)_localctx).arguments.add(((InstructionContext)_localctx).argument);
				setState(107);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while (_la==ArgumentSeparator) {
					{
					{
					setState(103);
					match(ArgumentSeparator);
					setState(104);
					((InstructionContext)_localctx).argument = argument();
					((InstructionContext)_localctx).arguments.add(((InstructionContext)_localctx).argument);
					}
					}
					setState(109);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				}
				break;
			}
			setState(115);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==StatementSeparator) {
				{
				{
				setState(112);
				match(StatementSeparator);
				}
				}
				setState(117);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(118);
			match(EndOfLine);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ArgumentContext extends ParserRuleContext {
		public Token value;
		public TerminalNode Integer() { return getToken(PasmParser.Integer, 0); }
		public TerminalNode Real() { return getToken(PasmParser.Real, 0); }
		public TerminalNode HexNumber() { return getToken(PasmParser.HexNumber, 0); }
		public TerminalNode String() { return getToken(PasmParser.String, 0); }
		public TerminalNode Register() { return getToken(PasmParser.Register, 0); }
		public ArgumentContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_argument; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof PasmListener ) ((PasmListener)listener).enterArgument(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof PasmListener ) ((PasmListener)listener).exitArgument(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof PasmVisitor ) return ((PasmVisitor<? extends T>)visitor).visitArgument(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ArgumentContext argument() throws RecognitionException {
		ArgumentContext _localctx = new ArgumentContext(_ctx, getState());
		enterRule(_localctx, 20, RULE_argument);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(120);
			((ArgumentContext)_localctx).value = _input.LT(1);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << Register) | (1L << Integer) | (1L << Real) | (1L << HexNumber) | (1L << String))) != 0)) ) {
				((ArgumentContext)_localctx).value = (Token)_errHandler.recoverInline(this);
			}
			else {
				if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
				_errHandler.reportMatch(this);
				consume();
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class LabelContext extends ParserRuleContext {
		public TerminalNode Integer() { return getToken(PasmParser.Integer, 0); }
		public TerminalNode LabelSuffix() { return getToken(PasmParser.LabelSuffix, 0); }
		public LabelContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_label; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof PasmListener ) ((PasmListener)listener).enterLabel(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof PasmListener ) ((PasmListener)listener).exitLabel(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof PasmVisitor ) return ((PasmVisitor<? extends T>)visitor).visitLabel(this);
			else return visitor.visitChildren(this);
		}
	}

	public final LabelContext label() throws RecognitionException {
		LabelContext _localctx = new LabelContext(_ctx, getState());
		enterRule(_localctx, 22, RULE_label);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(122);
			match(Integer);
			setState(123);
			match(LabelSuffix);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class DataContext extends ParserRuleContext {
		public TerminalNode DataPrefix() { return getToken(PasmParser.DataPrefix, 0); }
		public List<TerminalNode> HexDigitPair() { return getTokens(PasmParser.HexDigitPair); }
		public TerminalNode HexDigitPair(int i) {
			return getToken(PasmParser.HexDigitPair, i);
		}
		public TerminalNode EndOfLine() { return getToken(PasmParser.EndOfLine, 0); }
		public List<TerminalNode> StatementSeparator() { return getTokens(PasmParser.StatementSeparator); }
		public TerminalNode StatementSeparator(int i) {
			return getToken(PasmParser.StatementSeparator, i);
		}
		public DataContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_data; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof PasmListener ) ((PasmListener)listener).enterData(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof PasmListener ) ((PasmListener)listener).exitData(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof PasmVisitor ) return ((PasmVisitor<? extends T>)visitor).visitData(this);
			else return visitor.visitChildren(this);
		}
	}

	public final DataContext data() throws RecognitionException {
		DataContext _localctx = new DataContext(_ctx, getState());
		enterRule(_localctx, 24, RULE_data);
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(125);
			match(DataPrefix);
			setState(129);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==StatementSeparator) {
				{
				{
				setState(126);
				match(StatementSeparator);
				}
				}
				setState(131);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(132);
			match(HexDigitPair);
			setState(137);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,14,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					{
					{
					setState(133);
					match(StatementSeparator);
					setState(134);
					match(HexDigitPair);
					}
					} 
				}
				setState(139);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,14,_ctx);
			}
			setState(143);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==StatementSeparator) {
				{
				{
				setState(140);
				match(StatementSeparator);
				}
				}
				setState(145);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(146);
			match(EndOfLine);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class StringDataContext extends ParserRuleContext {
		public TerminalNode StringDataPrefix() { return getToken(PasmParser.StringDataPrefix, 0); }
		public TerminalNode String() { return getToken(PasmParser.String, 0); }
		public TerminalNode EndOfLine() { return getToken(PasmParser.EndOfLine, 0); }
		public List<TerminalNode> StatementSeparator() { return getTokens(PasmParser.StatementSeparator); }
		public TerminalNode StatementSeparator(int i) {
			return getToken(PasmParser.StatementSeparator, i);
		}
		public StringDataContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_stringData; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof PasmListener ) ((PasmListener)listener).enterStringData(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof PasmListener ) ((PasmListener)listener).exitStringData(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof PasmVisitor ) return ((PasmVisitor<? extends T>)visitor).visitStringData(this);
			else return visitor.visitChildren(this);
		}
	}

	public final StringDataContext stringData() throws RecognitionException {
		StringDataContext _localctx = new StringDataContext(_ctx, getState());
		enterRule(_localctx, 26, RULE_stringData);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(148);
			match(StringDataPrefix);
			setState(152);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==StatementSeparator) {
				{
				{
				setState(149);
				match(StatementSeparator);
				}
				}
				setState(154);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(155);
			match(String);
			setState(159);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==StatementSeparator) {
				{
				{
				setState(156);
				match(StatementSeparator);
				}
				}
				setState(161);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(162);
			match(EndOfLine);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class MaybeIdentifierContext extends ParserRuleContext {
		public TerminalNode Identifier() { return getToken(PasmParser.Identifier, 0); }
		public TerminalNode HexDigitPair() { return getToken(PasmParser.HexDigitPair, 0); }
		public TerminalNode Register() { return getToken(PasmParser.Register, 0); }
		public MaybeIdentifierContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_maybeIdentifier; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof PasmListener ) ((PasmListener)listener).enterMaybeIdentifier(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof PasmListener ) ((PasmListener)listener).exitMaybeIdentifier(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof PasmVisitor ) return ((PasmVisitor<? extends T>)visitor).visitMaybeIdentifier(this);
			else return visitor.visitChildren(this);
		}
	}

	public final MaybeIdentifierContext maybeIdentifier() throws RecognitionException {
		MaybeIdentifierContext _localctx = new MaybeIdentifierContext(_ctx, getState());
		enterRule(_localctx, 28, RULE_maybeIdentifier);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(164);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << Register) | (1L << HexDigitPair) | (1L << Identifier))) != 0)) ) {
			_errHandler.recoverInline(this);
			}
			else {
				if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
				_errHandler.reportMatch(this);
				consume();
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static final String _serializedATN =
		"\3\u608b\ua72a\u8133\ub9ed\u417c\u3be7\u7786\u5964\3\21\u00a9\4\2\t\2"+
		"\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4\13"+
		"\t\13\4\f\t\f\4\r\t\r\4\16\t\16\4\17\t\17\4\20\t\20\3\2\7\2\"\n\2\f\2"+
		"\16\2%\13\2\3\2\3\2\3\3\3\3\3\3\5\3,\n\3\3\4\3\4\3\4\3\5\3\5\3\5\3\6\3"+
		"\6\3\6\3\7\3\7\3\7\7\7:\n\7\f\7\16\7=\13\7\3\7\3\7\7\7A\n\7\f\7\16\7D"+
		"\13\7\3\b\3\b\7\bH\n\b\f\b\16\bK\13\b\6\bM\n\b\r\b\16\bN\3\t\3\t\7\tS"+
		"\n\t\f\t\16\tV\13\t\6\tX\n\t\r\t\16\tY\3\n\3\n\7\n^\n\n\f\n\16\na\13\n"+
		"\6\nc\n\n\r\n\16\nd\3\13\3\13\3\13\3\13\3\13\7\13l\n\13\f\13\16\13o\13"+
		"\13\5\13q\n\13\3\13\7\13t\n\13\f\13\16\13w\13\13\3\13\3\13\3\f\3\f\3\r"+
		"\3\r\3\r\3\16\3\16\7\16\u0082\n\16\f\16\16\16\u0085\13\16\3\16\3\16\3"+
		"\16\7\16\u008a\n\16\f\16\16\16\u008d\13\16\3\16\7\16\u0090\n\16\f\16\16"+
		"\16\u0093\13\16\3\16\3\16\3\17\3\17\7\17\u0099\n\17\f\17\16\17\u009c\13"+
		"\17\3\17\3\17\7\17\u00a0\n\17\f\17\16\17\u00a3\13\17\3\17\3\17\3\20\3"+
		"\20\3\20\2\2\21\2\4\6\b\n\f\16\20\22\24\26\30\32\34\36\2\5\3\2\17\20\4"+
		"\2\3\6\f\f\5\2\3\3\7\7\13\13\2\u00ac\2#\3\2\2\2\4+\3\2\2\2\6-\3\2\2\2"+
		"\b\60\3\2\2\2\n\63\3\2\2\2\f;\3\2\2\2\16L\3\2\2\2\20W\3\2\2\2\22b\3\2"+
		"\2\2\24f\3\2\2\2\26z\3\2\2\2\30|\3\2\2\2\32\177\3\2\2\2\34\u0096\3\2\2"+
		"\2\36\u00a6\3\2\2\2 \"\5\4\3\2! \3\2\2\2\"%\3\2\2\2#!\3\2\2\2#$\3\2\2"+
		"\2$&\3\2\2\2%#\3\2\2\2&\'\7\2\2\3\'\3\3\2\2\2(,\5\6\4\2),\5\b\5\2*,\5"+
		"\n\6\2+(\3\2\2\2+)\3\2\2\2+*\3\2\2\2,\5\3\2\2\2-.\5\f\7\2./\5\16\b\2/"+
		"\7\3\2\2\2\60\61\5\f\7\2\61\62\5\20\t\2\62\t\3\2\2\2\63\64\5\f\7\2\64"+
		"\65\5\22\n\2\65\13\3\2\2\2\66\67\5\30\r\2\678\7\20\2\28:\3\2\2\29\66\3"+
		"\2\2\2:=\3\2\2\2;9\3\2\2\2;<\3\2\2\2<>\3\2\2\2=;\3\2\2\2>B\5\30\r\2?A"+
		"\t\2\2\2@?\3\2\2\2AD\3\2\2\2B@\3\2\2\2BC\3\2\2\2C\r\3\2\2\2DB\3\2\2\2"+
		"EI\5\24\13\2FH\t\2\2\2GF\3\2\2\2HK\3\2\2\2IG\3\2\2\2IJ\3\2\2\2JM\3\2\2"+
		"\2KI\3\2\2\2LE\3\2\2\2MN\3\2\2\2NL\3\2\2\2NO\3\2\2\2O\17\3\2\2\2PT\5\32"+
		"\16\2QS\t\2\2\2RQ\3\2\2\2SV\3\2\2\2TR\3\2\2\2TU\3\2\2\2UX\3\2\2\2VT\3"+
		"\2\2\2WP\3\2\2\2XY\3\2\2\2YW\3\2\2\2YZ\3\2\2\2Z\21\3\2\2\2[_\5\34\17\2"+
		"\\^\t\2\2\2]\\\3\2\2\2^a\3\2\2\2_]\3\2\2\2_`\3\2\2\2`c\3\2\2\2a_\3\2\2"+
		"\2b[\3\2\2\2cd\3\2\2\2db\3\2\2\2de\3\2\2\2e\23\3\2\2\2fp\5\36\20\2gh\7"+
		"\17\2\2hm\5\26\f\2ij\7\21\2\2jl\5\26\f\2ki\3\2\2\2lo\3\2\2\2mk\3\2\2\2"+
		"mn\3\2\2\2nq\3\2\2\2om\3\2\2\2pg\3\2\2\2pq\3\2\2\2qu\3\2\2\2rt\7\17\2"+
		"\2sr\3\2\2\2tw\3\2\2\2us\3\2\2\2uv\3\2\2\2vx\3\2\2\2wu\3\2\2\2xy\7\20"+
		"\2\2y\25\3\2\2\2z{\t\3\2\2{\27\3\2\2\2|}\7\4\2\2}~\7\b\2\2~\31\3\2\2\2"+
		"\177\u0083\7\t\2\2\u0080\u0082\7\17\2\2\u0081\u0080\3\2\2\2\u0082\u0085"+
		"\3\2\2\2\u0083\u0081\3\2\2\2\u0083\u0084\3\2\2\2\u0084\u0086\3\2\2\2\u0085"+
		"\u0083\3\2\2\2\u0086\u008b\7\7\2\2\u0087\u0088\7\17\2\2\u0088\u008a\7"+
		"\7\2\2\u0089\u0087\3\2\2\2\u008a\u008d\3\2\2\2\u008b\u0089\3\2\2\2\u008b"+
		"\u008c\3\2\2\2\u008c\u0091\3\2\2\2\u008d\u008b\3\2\2\2\u008e\u0090\7\17"+
		"\2\2\u008f\u008e\3\2\2\2\u0090\u0093\3\2\2\2\u0091\u008f\3\2\2\2\u0091"+
		"\u0092\3\2\2\2\u0092\u0094\3\2\2\2\u0093\u0091\3\2\2\2\u0094\u0095\7\20"+
		"\2\2\u0095\33\3\2\2\2\u0096\u009a\7\n\2\2\u0097\u0099\7\17\2\2\u0098\u0097"+
		"\3\2\2\2\u0099\u009c\3\2\2\2\u009a\u0098\3\2\2\2\u009a\u009b\3\2\2\2\u009b"+
		"\u009d\3\2\2\2\u009c\u009a\3\2\2\2\u009d\u00a1\7\f\2\2\u009e\u00a0\7\17"+
		"\2\2\u009f\u009e\3\2\2\2\u00a0\u00a3\3\2\2\2\u00a1\u009f\3\2\2\2\u00a1"+
		"\u00a2\3\2\2\2\u00a2\u00a4\3\2\2\2\u00a3\u00a1\3\2\2\2\u00a4\u00a5\7\20"+
		"\2\2\u00a5\35\3\2\2\2\u00a6\u00a7\t\4\2\2\u00a7\37\3\2\2\2\24#+;BINTY"+
		"_dmpu\u0083\u008b\u0091\u009a\u00a1";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}