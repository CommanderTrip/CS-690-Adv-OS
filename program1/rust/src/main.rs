use std::fs::File;
use std::path::Path;

fn main() {
    // Get input file
    println!("Please enter the input file location or \"1\" to assume \"./InputFile\"");
    let file_location = Path::new("InputFile");

    let mut file = match File::open(&file_location) {
        Ok(file) => file,
        Err(why) => panic!("Couldn't open {}: {}", file_location.display(), why) 
    };

}
