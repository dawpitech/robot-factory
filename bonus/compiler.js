var opcodes = {"live" : 0x01,
               "ld"   : 0x02,
               "st"   : 0x03,
               "add"  : 0x04,
               "sub"  : 0x05,
               "and"  : 0x06,
               "or"   : 0x07,
               "xor"  : 0x08,
               "zjmp" : 0x09,
               "ldi"  : 0x0A,
               "sti"  : 0x0B,
               "fork" : 0x0C,
               "lld"  : 0x0D,
               "lldi" : 0x0F,
               "lfork": 0x10,
               "aff"  : 0x11
};

function toHex(d) {
    return  "0x" + ("0"+(Number(d).toString(16))).slice(-2).toUpperCase()
}

// consider there are up to r4 registers
const no_cb = ["live", "zjmp", "fork", "lfork"];
var input = "sti r2, 23, %34";
//var input = prompt();
console.log("input : " + input);
var output = "";

input = input.split(" ");
for (var i = 0; i < input.length; i++) {
  input[i] = input[i].replace(",", "");
}

output += toHex(opcodes[input[0]]) + " "; // getting the value of the opcode

var coding_byte = "";
if (!(no_cb.includes(input[0]))) {
    for (var j = 1; j < input.length; j++) {
      l = input[j];
      if (l[0] === 'r') { //register
        coding_byte += "01";
      }
      else if (l[0] === '%') { //direct
        coding_byte += "10";
      }
      else { //indirect
        coding_byte += "11";
      }
    }
    while (coding_byte.length < 8) {
        coding_byte += "00";
    }
    coding_byte = parseInt(coding_byte, 2);
    coding_byte = "0x" + coding_byte.toString(16) + " ";
}

output += coding_byte;

function encodeNumberAsString(num, byteCount) {
    // Check if the number is too big for the specified byteCount
    if (num >= Math.pow(256, byteCount)) {
        return "0x00";
    }

    var encodedString = "";
    for (var i = byteCount - 1; i >= 0; i--) {
        var byteValue = (num >> (i * 8)) & 0xFF;
        encodedString += "0x" + byteValue.toString(16).padStart(2, '0') + ' ';
    }
    return encodedString.trim();
}

// here indirects are 2 bytes long
// directs are 4 bytes long
// indirects are 1 byte long
var parameters = "";
for (var j = 1; j < input.length; j++) {
  l = input[j];
  if (l[0] === 'r') { //register
    l = l.replace("r", "");
    parameters += encodeNumberAsString(toHex(l), 1);
  }
  else if (l[0] === '%') { //direct
    l = l.replace("%", "");    
    parameters += encodeNumberAsString(toHex(l), 4);
  }
  else { //indirect
    parameters += encodeNumberAsString(toHex(l), 2);
  }
  parameters += ' ';
}

output += parameters;

console.log("compiled to bytecode: "+output);
