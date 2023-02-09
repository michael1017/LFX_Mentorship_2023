cd wasm_add; cargo build --release --target=wasm32-wasi; cd ..;
cd wasm_add3; cargo build --release --target=wasm32-wasi; cd ..;
cd wasm_circle_f32; cargo build --release --target=wasm32-wasi; cd ..;
cd wasm_circle_f64; cargo build --release --target=wasm32-wasi; cd ..;
cd wasm_hello; cargo build --release --target=wasm32-wasi; cd ..;

cp wasm_*/target/wasm32-wasi/release/*.wasm ./wasm