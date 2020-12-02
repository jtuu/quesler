package opcode_definitions

import assembler.*
import kotlin.math.abs
import kotlin.math.pow
import kotlin.math.sign
import kotlin.reflect.jvm.internal.ReflectProperties

class TypeException(expected: String, got: String) : Exception("Expected $expected but got $got")
class ValueException(message: String) : Exception(message)

abstract class OpcodeParameter {
    abstract fun validateArgument(arg: ArgumentNode<*>);
}

abstract class VariadicParameter(val itemType: OpcodeParameter) : OpcodeParameter() {
    override fun validateArgument(arg: ArgumentNode<*>) {
        itemType.validateArgument(arg)
    }
}

class AnyParameter : OpcodeParameter() {
    override fun validateArgument(arg: ArgumentNode<*>) {}
}

abstract class NumericParameter(val size: Int) : OpcodeParameter() {
    override fun validateArgument(arg: ArgumentNode<*>) {
        val value = when (arg) {
            is IntegerNode -> arg.value
            is RealNode -> arg.value
            else -> throw TypeException("a number", "a ${arg.name}")
        }

        if ((value < 0 && value < 2.0.pow(size * 8 - 1)) || value > 2.0.pow(size * 8)) {
            throw ValueException("Value $value too large to fit in $size bytes")
        }
    }
}

class ByteParameter : NumericParameter(1)
class WordParameter : NumericParameter(2)
class DwordParameter : NumericParameter(4)
class FloatParameter : NumericParameter(4)

open class LabelParameter : OpcodeParameter() {
    override fun validateArgument(arg: ArgumentNode<*>) {
        val value = when (arg) {
            is IntegerNode -> arg.value
            else -> throw TypeException("a label", "a ${arg.name}")
        }

        if (value < 0) {
            throw ValueException("Labels must be greater than or equal to 0")
        }
    }
}
class InstructionLabelParameter : LabelParameter()
class DataLabelParameter : LabelParameter()
class StringLabelParameter : LabelParameter()
class InstructionLabelListParameter : VariadicParameter(InstructionLabelParameter())

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
class RegisterReferenceListParameter(val access: OpcodeRegisterAccess) : VariadicParameter(AnyRegisterReferenceParameter(access))


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