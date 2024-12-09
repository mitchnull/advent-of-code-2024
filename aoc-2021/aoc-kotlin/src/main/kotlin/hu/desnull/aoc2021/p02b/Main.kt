package hu.desnull.aoc2021.p02b

private fun isWhiteSpace(c: Char) = c in " \r\n\t"
private fun readString() = generateSequence { System.`in`.read().toChar() }
  .dropWhile { isWhiteSpace(it) }.takeWhile { !isWhiteSpace(it) }.joinToString("")

fun main() {
  var x = 0
  var y = 0
  var aim = 0
  while (true) {
    val line = readlnOrNull() ?: break
    val (dir, cs) = line.split(" ")
    val c = cs.toInt()
    when (dir) {
      "forward" -> {
        x += c
        y += aim * c
      }
      "up" -> {
        aim -= c
      }
      "down" -> {
        aim += c
      }
      else -> {
        println("ERROR: $dir")
        return
      }
    }
  }
  println(x * y)
}