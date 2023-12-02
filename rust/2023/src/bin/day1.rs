use std::collections::HashMap;
use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;

fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>>
where
    P: AsRef<Path>,
{
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

fn part1(filename: &str) {
    let mut sum = 0;
    if let Ok(lines) = read_lines(filename) {
        for line in lines {
            if let Ok(line) = line {
                let mut digits = Vec::new();
                for c in line.chars() {
                    if c.is_ascii_digit() {
                        digits.push(c.to_digit(10).unwrap());
                    }
                }
                sum += digits.first().unwrap() * 10 + digits.last().unwrap();
            }
        }
    }
    println!("sum: {}", sum);
}

fn part2(filename: &str) {
    let mut sum = 0;
    let cases = HashMap::from([
        ("one", 1),
        ("two", 2),
        ("three", 3),
        ("four", 4),
        ("five", 5),
        ("six", 6),
        ("seven", 7),
        ("eight", 8),
        ("nine", 9),
    ]);

    if let Ok(lines) = read_lines(filename) {
        for line in lines {
            if let Ok(line) = line {
                let mut digits = Vec::new();
                for (i, c) in line.chars().enumerate() {
                    if c.is_ascii_digit() {
                        digits.push(c.to_digit(10).unwrap());
                    } else {
                        for (case, digit) in cases.iter() {
                            if i + case.len() <= line.len() {
                                let substring = &line[i..i + case.len()];
                                if substring == *case {
                                    digits.push(*digit);
                                }
                            }
                        }
                    }
                }
                println!("{}: {:?}", line, digits);
                sum += digits.first().unwrap() * 10 + digits.last().unwrap();
            }
        }
    }
    println!("sum: {}", sum);
}

fn main() {
    //part1("../../inputs/2023/day1/part1_test");
    //part1("../../inputs/2023/day1/data");
    //part2("../../inputs/2023/day1/part2_test");
    part2("../../inputs/2023/day1/data");
}
