#![no_std]
#![crate_type="staticlib"]


// **************************************
// These are here just to make the linker happy
// These functions are just used for critical error handling so for now we just loop forever
// For more information see: https://github.com/rust-lang/rust/blob/master/src/doc/trpl/unsafe.md

#![feature(lang_items)]

//extern crate core;

// #[lang="stack_exhausted"] extern fn stack_exhausted() {}
#[lang="eh_personality"] extern fn eh_personality() {}

#[lang="panic_fmt"]
pub fn panic_fmt(_fmt: &core::fmt::Arguments, _file_line: &(&'static str, usize)) -> ! {
loop { }
}

#[no_mangle]
pub unsafe fn __aeabi_unwind_cpp_pr0() -> () {
loop {}
}

// **************************************
// **************************************

// And now we can write some Rust!


#[no_mangle]
pub fn rust_main() -> (u8) {

// Do stuff here
return 1;

}
