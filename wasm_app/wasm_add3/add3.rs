#[no_mangle]
fn _start(a: i64, b: i64, c: i64) -> i64 {
  return add3(a, b, c);
}

#[no_mangle]
fn add3(a: i64, b: i64, c: i64) -> i64 {
  return a + b + c;
}
