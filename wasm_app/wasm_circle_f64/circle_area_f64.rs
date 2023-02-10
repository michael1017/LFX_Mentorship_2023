#[no_mangle]
fn _start(r: f64) -> f64 {
    return circle_area(r);
}

#[no_mangle]
fn circle_area(r: f64) -> f64 {
    return r * r * std::f64::consts::PI;
}