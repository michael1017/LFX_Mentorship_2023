#[no_mangle]
fn _start(r: f32) -> f32 {
    return circle_area(r);
}

#[no_mangle]
fn circle_area(r: f32) -> f32 {
    return r * r * std::f32::consts::PI;
}