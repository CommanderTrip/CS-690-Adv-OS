use core::time;
use std::fs::{File, OpenOptions};
use std::io::{self, BufRead, Write};
use std::path::Path;
use std::sync::{Arc, Mutex};
use std::thread::{self, sleep};

/**
 * Program written by Jason Carnahan as a Rust version of the CS 690 program 1.
 * See the `program1.pdf` requirements document for details.
 *
 * The program limits the use of the `.unwrap()` for the sake of custom error handling.
 */
fn main() {
    // Get input file and open for reading
    let file_location = Path::new("InputFile");
    let file = match File::open(&file_location) {
        Ok(file) => file,
        Err(why) => panic!("Couldn't open {}: {}", file_location.display(), why),
    };

    // Read in the lines of the file, parse name and wait time, and store in a vector
    let mut process_vector: Vec<Process> = Vec::new();
    let lines = io::BufReader::new(file).lines();
    for (index, line) in lines.flatten().into_iter().enumerate() {
        let split: Vec<&str> = line.split(" ").collect();

        let process = Process {
            thread_id: index + 1,
            process_name: split[0].to_string(),
            wait_time: match split[1].parse::<i32>() {
                Ok(num) => num,
                Err(_) => panic!("Could not parse wait time."),
            },
        };

        process_vector.push(process);
    }

    // Open the output file and create and mutex that can safely pass among the threads
    let output_file = OpenOptions::new()
        .create(true)
        .append(true)
        .open("OutputFile")
        .expect("Failed accessing output file");
    let writer = Arc::new(Mutex::new(output_file));

    // Spawn a thread for each "process" and print the data using a Mutex
    let num_of_threads = process_vector.len();
    let mut thread_handles = vec![]; // Storage to wait for each thread to finish
    for i in 0..num_of_threads {
        let writer = Arc::clone(&writer); // A mutex for each thread
        let process = process_vector[i].clone();
        let thread_handle = thread::spawn(move || {
            scheduled_process(process, &writer);
        });
        thread_handles.push(thread_handle);
    }

    for thread in thread_handles {
        thread.join().unwrap();
    }
}

#[derive(Clone)]
struct Process {
    thread_id: usize,
    process_name: String,
    wait_time: i32,
}

fn scheduled_process(process: Process, writer: &Mutex<File>) {
    // Greet the start of the thread
    writer
        .lock()
        .unwrap()
        .write(
            format!(
                "Thread{}: {} begins\n",
                process.thread_id, process.process_name
            )
            .as_bytes(),
        )
        .expect("Could not write to outputfile.");

    // Loop for the wait time and print status
    let num_of_loops = process.wait_time / 5;
    for i in 1..num_of_loops + 1 {
        writer
            .lock()
            .unwrap()
            .write(
                format!(
                    "Thread{}: Loop {} of {} loops total\n",
                    process.thread_id, i, num_of_loops
                )
                .as_bytes(),
            )
            .expect("Could not write to outputfile.");
        sleep(time::Duration::from_micros(5000));
    }

    // End the thread
    writer
        .lock()
        .unwrap()
        .write(
            format!(
                "Thread{}: {} ends\n",
                process.thread_id, process.process_name
            )
            .as_bytes(),
        )
        .expect("Could not write to outputfile.");
}
