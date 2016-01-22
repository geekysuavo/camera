$1 == a {
  print ($2 * 2), $5
  f = $5
}
END {
  for (i = 201; i <= 500; i++) {
    print (i * 2), f
  }
}
