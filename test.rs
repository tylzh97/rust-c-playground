use std::os::raw::{c_int, c_float, c_longlong};

#[no_mangle]
pub unsafe extern "C" fn sum_elements(array: *const c_int, len: c_int, float_number: c_float) -> *const c_longlong {
    let array = std::slice::from_raw_parts(array, len as usize);
    let mut results: Vec<c_longlong> = Vec::new();

    println!("Rust Recv Array Length: {} ", len);

    for &num in array {
        results.push(((num as f32) + float_number) as c_longlong);
    }
    
    let result_ptr = results.as_ptr();
    std::mem::forget(results);
    result_ptr
}

#[no_mangle]
pub extern "C" fn free_memory(ptr: *mut c_longlong) {
    unsafe {
        drop(Box::from_raw(ptr));  // 重新获得所有权并删除Box，这将自动释放内存
    }
    println!("Rust Free!");
}
