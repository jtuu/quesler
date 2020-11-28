// Generated from /home/joona/realhome/quesler/src/assembler/Pasm.g4 by ANTLR 4.8
package assembler.parser;
import org.antlr.v4.runtime.Lexer;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.Token;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.misc.*;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class PasmLexer extends Lexer {
	static { RuntimeMetaData.checkVersion("4.8", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		Register=1, Integer=2, Real=3, HexNumber=4, HexDigitPair=5, LabelSuffix=6, 
		DataPrefix=7, StringDataPrefix=8, Identifier=9, String=10, SingleQuoteString=11, 
		DoubleQuoteString=12, StatementSeparator=13, EndOfLine=14, ArgumentSeparator=15;
	public static String[] channelNames = {
		"DEFAULT_TOKEN_CHANNEL", "HIDDEN"
	};

	public static String[] modeNames = {
		"DEFAULT_MODE"
	};

	private static String[] makeRuleNames() {
		return new String[] {
			"Register", "RegisterPrefix", "Integer", "Real", "HexNumber", "HexDigitPair", 
			"LabelSuffix", "DataPrefix", "StringDataPrefix", "Identifier", "IdentifierPrefix", 
			"Letter", "IdentifierSpecialCharacter", "DecimalSeparator", "HexNumberPrefix", 
			"NegativeNumberPrefix", "Digit", "NonZeroDigit", "ZeroDigit", "HexDigit", 
			"String", "SingleQuoteString", "DoubleQuoteString", "StatementSeparator", 
			"EndOfLine", "ArgumentSeparator"
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


	public PasmLexer(CharStream input) {
		super(input);
		_interp = new LexerATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@Override
	public String getGrammarFileName() { return "Pasm.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public String[] getChannelNames() { return channelNames; }

	@Override
	public String[] getModeNames() { return modeNames; }

	@Override
	public ATN getATN() { return _ATN; }

	public static final String _serializedATN =
		"\3\u608b\ua72a\u8133\ub9ed\u417c\u3be7\u7786\u5964\2\21\u00b3\b\1\4\2"+
		"\t\2\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4"+
		"\13\t\13\4\f\t\f\4\r\t\r\4\16\t\16\4\17\t\17\4\20\t\20\4\21\t\21\4\22"+
		"\t\22\4\23\t\23\4\24\t\24\4\25\t\25\4\26\t\26\4\27\t\27\4\30\t\30\4\31"+
		"\t\31\4\32\t\32\4\33\t\33\3\2\3\2\3\2\3\3\3\3\3\4\5\4>\n\4\3\4\3\4\3\4"+
		"\7\4C\n\4\f\4\16\4F\13\4\5\4H\n\4\3\5\3\5\3\5\6\5M\n\5\r\5\16\5N\3\6\3"+
		"\6\6\6S\n\6\r\6\16\6T\3\7\3\7\3\7\3\b\3\b\3\t\3\t\3\t\3\t\3\t\3\n\3\n"+
		"\3\n\3\n\3\n\3\13\3\13\3\13\7\13i\n\13\f\13\16\13l\13\13\3\f\3\f\5\fp"+
		"\n\f\3\r\3\r\3\16\3\16\3\17\3\17\3\20\3\20\3\20\3\21\3\21\3\22\3\22\3"+
		"\23\3\23\3\24\3\24\3\25\3\25\3\26\3\26\5\26\u0087\n\26\3\27\3\27\7\27"+
		"\u008b\n\27\f\27\16\27\u008e\13\27\3\27\3\27\3\30\3\30\7\30\u0094\n\30"+
		"\f\30\16\30\u0097\13\30\3\30\3\30\3\31\6\31\u009c\n\31\r\31\16\31\u009d"+
		"\3\32\3\32\3\32\6\32\u00a3\n\32\r\32\16\32\u00a4\3\33\7\33\u00a8\n\33"+
		"\f\33\16\33\u00ab\13\33\3\33\3\33\7\33\u00af\n\33\f\33\16\33\u00b2\13"+
		"\33\2\2\34\3\3\5\2\7\4\t\5\13\6\r\7\17\b\21\t\23\n\25\13\27\2\31\2\33"+
		"\2\35\2\37\2!\2#\2%\2\'\2)\2+\f-\r/\16\61\17\63\20\65\21\3\2\13\4\2TT"+
		"tt\4\2C\\c|\7\2##,-//>Aaa\3\2\62;\3\2\63;\5\2\62;CHch\4\2\f\f))\4\2\f"+
		"\f$$\4\2\13\13\"\"\2\u00b7\2\3\3\2\2\2\2\7\3\2\2\2\2\t\3\2\2\2\2\13\3"+
		"\2\2\2\2\r\3\2\2\2\2\17\3\2\2\2\2\21\3\2\2\2\2\23\3\2\2\2\2\25\3\2\2\2"+
		"\2+\3\2\2\2\2-\3\2\2\2\2/\3\2\2\2\2\61\3\2\2\2\2\63\3\2\2\2\2\65\3\2\2"+
		"\2\3\67\3\2\2\2\5:\3\2\2\2\7=\3\2\2\2\tI\3\2\2\2\13P\3\2\2\2\rV\3\2\2"+
		"\2\17Y\3\2\2\2\21[\3\2\2\2\23`\3\2\2\2\25e\3\2\2\2\27o\3\2\2\2\31q\3\2"+
		"\2\2\33s\3\2\2\2\35u\3\2\2\2\37w\3\2\2\2!z\3\2\2\2#|\3\2\2\2%~\3\2\2\2"+
		"\'\u0080\3\2\2\2)\u0082\3\2\2\2+\u0086\3\2\2\2-\u0088\3\2\2\2/\u0091\3"+
		"\2\2\2\61\u009b\3\2\2\2\63\u00a2\3\2\2\2\65\u00a9\3\2\2\2\678\5\5\3\2"+
		"89\5\7\4\29\4\3\2\2\2:;\t\2\2\2;\6\3\2\2\2<>\5!\21\2=<\3\2\2\2=>\3\2\2"+
		"\2>G\3\2\2\2?H\5\'\24\2@D\5%\23\2AC\5#\22\2BA\3\2\2\2CF\3\2\2\2DB\3\2"+
		"\2\2DE\3\2\2\2EH\3\2\2\2FD\3\2\2\2G?\3\2\2\2G@\3\2\2\2H\b\3\2\2\2IJ\5"+
		"\7\4\2JL\5\35\17\2KM\5#\22\2LK\3\2\2\2MN\3\2\2\2NL\3\2\2\2NO\3\2\2\2O"+
		"\n\3\2\2\2PR\5\37\20\2QS\5)\25\2RQ\3\2\2\2ST\3\2\2\2TR\3\2\2\2TU\3\2\2"+
		"\2U\f\3\2\2\2VW\5)\25\2WX\5)\25\2X\16\3\2\2\2YZ\7<\2\2Z\20\3\2\2\2[\\"+
		"\7J\2\2\\]\7G\2\2]^\7Z\2\2^_\7<\2\2_\22\3\2\2\2`a\7U\2\2ab\7V\2\2bc\7"+
		"T\2\2cd\7<\2\2d\24\3\2\2\2ej\5\27\f\2fi\5\27\f\2gi\5#\22\2hf\3\2\2\2h"+
		"g\3\2\2\2il\3\2\2\2jh\3\2\2\2jk\3\2\2\2k\26\3\2\2\2lj\3\2\2\2mp\5\31\r"+
		"\2np\5\33\16\2om\3\2\2\2on\3\2\2\2p\30\3\2\2\2qr\t\3\2\2r\32\3\2\2\2s"+
		"t\t\4\2\2t\34\3\2\2\2uv\7\60\2\2v\36\3\2\2\2wx\7\62\2\2xy\7z\2\2y \3\2"+
		"\2\2z{\7/\2\2{\"\3\2\2\2|}\t\5\2\2}$\3\2\2\2~\177\t\6\2\2\177&\3\2\2\2"+
		"\u0080\u0081\7\62\2\2\u0081(\3\2\2\2\u0082\u0083\t\7\2\2\u0083*\3\2\2"+
		"\2\u0084\u0087\5-\27\2\u0085\u0087\5/\30\2\u0086\u0084\3\2\2\2\u0086\u0085"+
		"\3\2\2\2\u0087,\3\2\2\2\u0088\u008c\7)\2\2\u0089\u008b\n\b\2\2\u008a\u0089"+
		"\3\2\2\2\u008b\u008e\3\2\2\2\u008c\u008a\3\2\2\2\u008c\u008d\3\2\2\2\u008d"+
		"\u008f\3\2\2\2\u008e\u008c\3\2\2\2\u008f\u0090\7)\2\2\u0090.\3\2\2\2\u0091"+
		"\u0095\7$\2\2\u0092\u0094\n\t\2\2\u0093\u0092\3\2\2\2\u0094\u0097\3\2"+
		"\2\2\u0095\u0093\3\2\2\2\u0095\u0096\3\2\2\2\u0096\u0098\3\2\2\2\u0097"+
		"\u0095\3\2\2\2\u0098\u0099\7$\2\2\u0099\60\3\2\2\2\u009a\u009c\t\n\2\2"+
		"\u009b\u009a\3\2\2\2\u009c\u009d\3\2\2\2\u009d\u009b\3\2\2\2\u009d\u009e"+
		"\3\2\2\2\u009e\62\3\2\2\2\u009f\u00a0\7\17\2\2\u00a0\u00a3\7\f\2\2\u00a1"+
		"\u00a3\7\f\2\2\u00a2\u009f\3\2\2\2\u00a2\u00a1\3\2\2\2\u00a3\u00a4\3\2"+
		"\2\2\u00a4\u00a2\3\2\2\2\u00a4\u00a5\3\2\2\2\u00a5\64\3\2\2\2\u00a6\u00a8"+
		"\5\61\31\2\u00a7\u00a6\3\2\2\2\u00a8\u00ab\3\2\2\2\u00a9\u00a7\3\2\2\2"+
		"\u00a9\u00aa\3\2\2\2\u00aa\u00ac\3\2\2\2\u00ab\u00a9\3\2\2\2\u00ac\u00b0"+
		"\7.\2\2\u00ad\u00af\5\61\31\2\u00ae\u00ad\3\2\2\2\u00af\u00b2\3\2\2\2"+
		"\u00b0\u00ae\3\2\2\2\u00b0\u00b1\3\2\2\2\u00b1\66\3\2\2\2\u00b2\u00b0"+
		"\3\2\2\2\23\2=DGNThjo\u0086\u008c\u0095\u009d\u00a2\u00a4\u00a9\u00b0"+
		"\2";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}