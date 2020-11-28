grammar Pasm;

program: (blocks+=block)* EOF;

block: (codeBlock | dataBlock | stringBlock);
codeBlock: labelList codeList;
dataBlock: labelList dataList;
stringBlock: labelList stringList;
labelList: (label EndOfLine)* label (StatementSeparator | EndOfLine)*;
codeList: (instruction (StatementSeparator | EndOfLine)*)+;
dataList: (data (StatementSeparator | EndOfLine)*)+;
stringList: (stringData (StatementSeparator | EndOfLine)*)+;

instruction: opcode=maybeIdentifier (StatementSeparator arguments+=argument (ArgumentSeparator arguments+=argument)*)? StatementSeparator* EndOfLine;
argument: value = (Integer | Real | HexNumber | String | Register);

Register: RegisterPrefix Integer;
fragment RegisterPrefix: 'R' | 'r';

Integer: NegativeNumberPrefix? (ZeroDigit | NonZeroDigit Digit*);
Real: Integer DecimalSeparator Digit+;
HexNumber: HexNumberPrefix HexDigit+;
HexDigitPair: HexDigit HexDigit;

label: Integer LabelSuffix;
LabelSuffix: ':';

DataPrefix: 'HEX:';
StringDataPrefix: 'STR:';
data: DataPrefix StatementSeparator* HexDigitPair (StatementSeparator HexDigitPair)* StatementSeparator* EndOfLine;
stringData: StringDataPrefix StatementSeparator* String StatementSeparator* EndOfLine;

maybeIdentifier: Identifier | HexDigitPair | Register;
Identifier: IdentifierPrefix (IdentifierPrefix | Digit)*;
fragment IdentifierPrefix: Letter | IdentifierSpecialCharacter;
fragment Letter: [A-Za-z];
fragment IdentifierSpecialCharacter: [+\-_*=<>!?];

fragment DecimalSeparator: '.';
fragment HexNumberPrefix: '0x';
fragment NegativeNumberPrefix: '-';

fragment Digit: [0-9];
fragment NonZeroDigit: [1-9];
fragment ZeroDigit: '0';
fragment HexDigit: [A-Fa-f0-9];

String: SingleQuoteString | DoubleQuoteString;
SingleQuoteString: '\'' ~('\'' | '\n')* '\'';
DoubleQuoteString: '"' ~('"' | '\n')* '"';

StatementSeparator: (' ' | '\t')+;
EndOfLine: ('\r\n' | '\n')+;
ArgumentSeparator: StatementSeparator* ',' StatementSeparator*;
