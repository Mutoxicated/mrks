
pub struct Consumer;
impl Consumer {
    pub fn take_until(string:&str, start:usize, condition:u8) -> String {
        let mut index = start;
        
        let strlen = string.len();
        let bytes = string.as_bytes();

        let mut buf:Vec<u8> = Vec::new();
        buf.reserve(4);
        
        let mut c:u8;
        loop {
            c = bytes[index];
            if c == condition {
                break;
            }
            buf.push(c);
            index += 1;
            if index > strlen {
                break;
            }
        }
        
        String::from_utf8(buf).unwrap()
    }
}