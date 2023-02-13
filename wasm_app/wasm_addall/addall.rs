use std::env;

#[no_mangle]
fn addall() -> i64 {
  let mut sum: i64 = 0;
  for arg in env::args().skip(1) {
    if let Ok(x) = arg.parse::<i64>() {
      sum = sum + x;
    } else {
        println!("Found Non-Integer!");
    }
  }
  return sum;
}

#[no_mangle]
fn _start() -> i64 {
  return addall();
}


