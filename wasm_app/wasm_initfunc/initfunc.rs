#[no_mangle]
fn _start() -> i32 {
    println!("run start");
    return 2023;
}

#[no_mangle]
fn _initialize() {
    println!("run init");
}