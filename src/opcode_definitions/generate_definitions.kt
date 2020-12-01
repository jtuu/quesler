package opcode_definitions

import org.yaml.snakeyaml.Yaml
import java.io.File
import java.lang.Exception
import java.lang.StringBuilder
import java.time.LocalDateTime

enum class ParamTypeYaml {
    any,
    byte,
    word,
    dword,
    float,
    label,
    instruction_label,
    data_label,
    string_label,
    string,
    instruction_label_var,
    reg_ref,
    reg_tup_ref,
    reg_ref_var,
    pointer
}

enum class ParamAccessYaml {
    none,
    write,
    read,
    read_write
}

enum class StackAccessYaml {
    none,
    push,
    pop
}

class RegYaml {
    var name: String = ""
    var type: ParamTypeYaml = ParamTypeYaml.any
    var access: ParamAccessYaml = ParamAccessYaml.none
    var doc: String = ""
}

class ParamYaml {
    var name: String = ""
    var type: ParamTypeYaml = ParamTypeYaml.any
    var reg_tup: List<RegYaml> = listOf()
    var access: ParamAccessYaml = ParamAccessYaml.none
    var doc: String = ""
}

class OpcodeDefYaml {
    var code: Int = -1
    var mnemonic: String = ""
    lateinit var params: List<ParamYaml>
    var doc: String = ""
    var stack: StackAccessYaml = StackAccessYaml.none
}

class OpcodeListYaml {
    lateinit var opcodes: List<OpcodeDefYaml>
}

private fun enumizeMnemonic(mnemonic: String): String {
    return mnemonic.replace("!=", "ne")
            .replace("<=", "le")
            .replace(">=", "ge")
            .replace("<", "l")
            .replace(">", "g")
            .replace("=", "e")
            .toUpperCase()
}

private fun addEnumLine(stringBuilder: StringBuilder, name: String, value: Int) {
    stringBuilder.append(name, "(", "0x", value.toString(radix = 16), ")")
}

private fun genParamAccess(access: ParamAccessYaml): String {
    return when (access) {
        ParamAccessYaml.none -> "OpcodeRegisterAccess.None"
        ParamAccessYaml.read -> "OpcodeRegisterAccess.Read"
        ParamAccessYaml.write -> "OpcodeRegisterAccess.Write"
        ParamAccessYaml.read_write -> "OpcodeRegisterAccess.ReadWrite"
    }
}

private fun genRegTupRefParam(reg: RegYaml): String {
    val paramClassName = when (reg.type) {
        ParamTypeYaml.any -> "AnyRegisterReferenceParameter"
        ParamTypeYaml.byte -> "ByteRegisterReferenceParameter"
        ParamTypeYaml.word -> "WordRegisterReferenceParameter"
        ParamTypeYaml.dword -> "DwordRegisterReferenceParameter"
        ParamTypeYaml.float -> "FloatRegisterReferenceParameter"
        ParamTypeYaml.instruction_label -> "InstructionLabelRegisterReferenceParameter"
        ParamTypeYaml.pointer -> "PointerRegisterReferenceParameter"
        else -> throw Exception("Unrecognized typed register reference \"${reg.type.name}\"")
    }

    return "$paramClassName(${genParamAccess(reg.access)})"
}

private fun genRegTupRefParams(regs: List<RegYaml>): String {
    if (regs.isEmpty()) {
        return ""
    }

    var str = genRegTupRefParam(regs[0])
    for (i in 1 until regs.size) {
        str += ", " + genRegTupRefParam(regs[i])
    }

    return str
}

private fun addParam(stringBuilder: StringBuilder, param: ParamYaml) {
    stringBuilder.append(when (param.type) {
        ParamTypeYaml.any -> "AnyParameter()"
        ParamTypeYaml.byte -> "ByteParameter()"
        ParamTypeYaml.word -> "WordParameter()"
        ParamTypeYaml.dword -> "DwordParameter()"
        ParamTypeYaml.float -> "FloatParameter()"
        ParamTypeYaml.label -> "LabelParameter()"
        ParamTypeYaml.instruction_label -> "InstructionLabelParameter()"
        ParamTypeYaml.data_label -> "DataLabelParameter()"
        ParamTypeYaml.string_label -> "StringLabelParameter()"
        ParamTypeYaml.string -> "StringParameter()"
        ParamTypeYaml.instruction_label_var -> "InstructionLabelListParameter()"
        ParamTypeYaml.reg_ref -> "AnyRegisterReferenceParameter(${genParamAccess(param.access)})"
        ParamTypeYaml.reg_tup_ref -> "RegisterTupleReferenceParameter(listOf(${genRegTupRefParams(param.reg_tup)}))"
        ParamTypeYaml.reg_ref_var -> "RegisterReferenceListParameter(${genParamAccess(param.access)})"
        ParamTypeYaml.pointer -> "PointerParameter"
    })
}

private fun addOpcode(stringBuilder: StringBuilder, op: OpcodeDefYaml) {
    var mnemonic = op.mnemonic
    if (mnemonic.isEmpty()) {
        mnemonic = "unknown_" + op.code.toString(radix = 16)
        op.mnemonic = mnemonic
    }

    val stackOperationString = when (op.stack) {
        StackAccessYaml.none -> "None"
        StackAccessYaml.pop -> "Pop"
        StackAccessYaml.push -> "Push"
    }

    stringBuilder.append(
            "OpcodeDefinition(Opcode.", enumizeMnemonic(op.mnemonic),
            ", \"", mnemonic, "\", ",
            "OpcodeStackOperation.", stackOperationString, ", ",
            "listOf(")

    if (op.params.isNotEmpty()) {
        addParam(stringBuilder, op.params[0])

        for (i in 1 until op.params.size) {
            stringBuilder.append(", ")
            addParam(stringBuilder, op.params[i])
        }
    }

    stringBuilder.append("))")
}

private fun generateOpcodeDefinitions() {
    val opcodeData = Yaml()
    val opcodes = opcodeData
            .loadAs(File("opcodes.yml").inputStream(), OpcodeListYaml::class.java)
            .opcodes

    if (opcodes.isEmpty()) {
        return
    }

    val opcodesStr = StringBuilder("val opcodes = listOf(\n    ")
    addOpcode(opcodesStr, opcodes[0])

    val opcodeEnumStr = StringBuilder("enum class Opcode(val value: Int) {\n    ")
    addEnumLine(opcodeEnumStr, enumizeMnemonic(opcodes[0].mnemonic), opcodes[0].code)

    for (i in 1 until opcodes.size) {
        opcodesStr.append(",\n    ")
        try {
            addOpcode(opcodesStr, opcodes[i])
        } catch (e: Exception) {
            throw Exception("Failed to process opcode ${opcodes[i].mnemonic}: ${e.message}")
        }

        opcodeEnumStr.append(",\n    ")
        addEnumLine(opcodeEnumStr, enumizeMnemonic(opcodes[i].mnemonic), opcodes[i].code)
    }

    opcodesStr.append("\n)\n")

    opcodeEnumStr.append("\n}\n")

    val writer = File("opcodes.kt").bufferedWriter()
    writer.write("// File generated by generate_definitions.kt on ${LocalDateTime.now()}\n")
    writer.write("package opcode_definitions\n")
    writer.write(opcodeEnumStr.toString())
    writer.write(opcodesStr.toString())
    writer.close()
}

fun main() {
    generateOpcodeDefinitions()
}