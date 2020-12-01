package opcode_definitions

abstract class OpcodeParameter

class AnyParameter : OpcodeParameter()

abstract class NumericParameter(val size: Int) : OpcodeParameter()
class ByteParameter : NumericParameter(1)
class WordParameter : NumericParameter(2)
class DwordParameter : NumericParameter(4)
class FloatParameter : NumericParameter(4)

open class LabelParameter : OpcodeParameter()
class InstructionLabelParameter : LabelParameter()
class DataLabelParameter : LabelParameter()
class StringLabelParameter : LabelParameter()
class InstructionLabelListParameter : OpcodeParameter()

class StringParameter : OpcodeParameter()

abstract class RegisterReferenceParameter(val access: OpcodeRegisterAccess) : OpcodeParameter()
class AnyRegisterReferenceParameter(access: OpcodeRegisterAccess) : RegisterReferenceParameter(access)
class ByteRegisterReferenceParameter(access: OpcodeRegisterAccess) : RegisterReferenceParameter(access)
class WordRegisterReferenceParameter(access: OpcodeRegisterAccess) : RegisterReferenceParameter(access)
class DwordRegisterReferenceParameter(access: OpcodeRegisterAccess) : RegisterReferenceParameter(access)
class FloatRegisterReferenceParameter(access: OpcodeRegisterAccess) : RegisterReferenceParameter(access)
open class LabelRegisterReferenceParameter(access: OpcodeRegisterAccess): RegisterReferenceParameter(access)
class InstructionLabelRegisterReferenceParameter(access: OpcodeRegisterAccess): LabelRegisterReferenceParameter(access)
class PointerRegisterReferenceParameter(access: OpcodeRegisterAccess): RegisterReferenceParameter(access)

class RegisterTupleReferenceParameter(val tuple: List<RegisterReferenceParameter>) : OpcodeParameter()
class RegisterReferenceListParameter(val access: OpcodeRegisterAccess) : OpcodeParameter()


class PointerParameter : OpcodeParameter()

enum class OpcodeStackOperation {
    None,
    Push,
    Pop
}

enum class OpcodeRegisterAccess {
    None,
    Read,
    Write,
    ReadWrite
}

data class OpcodeDefinition(val opcode: Opcode,
                            val mnemonic: String,
                            val stackOperation: OpcodeStackOperation,
                            val parameters: List<OpcodeParameter>)