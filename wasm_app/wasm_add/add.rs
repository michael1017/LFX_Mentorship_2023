#[no_mangle]
fn _start(a: i32, b: i32) -> i32 {
  return add(a, b);
}

#[no_mangle]
fn add(a: i32, b: i32) -> i32 {
  return a + b;
}