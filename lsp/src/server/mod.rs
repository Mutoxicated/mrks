use std::{io::{self, BufRead, BufReader, Error, Read}, net::{TcpListener, TcpStream, UdpSocket}, sync::mpsc::{channel, Receiver, Sender}, time::Duration};

struct Message {
    value: serde_json::Value
}

struct Connection {
    sender: Sender<Message>,
    receiver: Receiver<Message>
}

impl Connection {
}

#[macro_export]
macro_rules! ADDRESS {
    () => {
        "127.0.0.1:30000"
    };
}

macro_rules! MESSAGE_CAP {
    () => {
        7000
    };
}

pub fn run() -> Result<(), Error> {
    let socket = TcpListener::bind(ADDRESS!())?;
    let stream = socket.accept()?;

    println!("Accepted connection from {}", stream.1);
    let mut buf:String = String::new();
    let mut stream = stream.0;
    loop {
        let mut received: Vec<u8> = vec![];
        let mut rx_bytes = [0u8; MESSAGE_CAP!()];
        loop {
            let bytes_read = stream.read(&mut rx_bytes)?;

            received.extend_from_slice(&rx_bytes[..bytes_read]);

            if bytes_read < MESSAGE_CAP!() {
                break;
            }
        }
        buf = String::from_utf8_lossy(&rx_bytes).into_owned();
        if buf == "" {
            continue;
        }
        println!("Got {buf}");
    }
}