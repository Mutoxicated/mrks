
pub struct Consumer {
    pub step: isize
}

impl Default for Consumer {
    fn default() -> Self {
        Self {
            step:1
        }
    }
}

impl Consumer {
    pub fn new(step:isize) -> Self {
        Self {
            step
        }
    }

    pub fn take_until_exclude(&self, string:&str, start:usize, conditions:Vec<char>) -> String {
        let start:isize = start.try_into().expect(format!("Consumer failed: start given was {start}").as_str());
        let mut index = start;
        let conditions:Vec<u8> = conditions.iter().map(|c| {
            c.clone() as u8
        }).collect();
        
        let strlen = string.len() as isize;
        let bytes = string.as_bytes();

        let mut buf:Vec<u8> = Vec::new();
        buf.reserve(4);
        
        let mut c:u8;
        loop {
            c = bytes[index as usize];
            if conditions.iter().any(|b| *b == c) {
                break;
            }
            buf.push(c);
            index += self.step;
            if index > strlen || index < 0 {
                break;
            }
        }
        if self.step < 0 {
            buf.reverse();
        }
        String::from_utf8(buf).unwrap()
    }

    pub fn take_until(&self, string:&str, start:usize, conditions:Vec<char>) -> String {
        let start:isize = start.try_into().expect(format!("Consumer failed: start given was {start}").as_str());
        let mut index = start;
        let conditions:Vec<u8> = conditions.iter().map(|c| {
            c.clone() as u8
        }).collect();
        
        let strlen = string.len() as isize;
        let bytes = string.as_bytes();

        let mut buf:Vec<u8> = Vec::new();
        buf.reserve(4);
        
        let mut c:u8;
        loop {
            c = bytes[index as usize];
            buf.push(c);
            if conditions.iter().any(|b| *b == c) {
                break;
            }
            index += self.step;
            if index >= strlen || index < 0 {
                break;
            }
        }
        if self.step < 0 {
            buf.reverse();
        }
        String::from_utf8(buf).unwrap()
    }
}