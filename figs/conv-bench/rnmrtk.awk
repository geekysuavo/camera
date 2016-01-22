/^Cnew/ && NR >= a && NR <= b {
  print (n * 6), $6
  n++
}
