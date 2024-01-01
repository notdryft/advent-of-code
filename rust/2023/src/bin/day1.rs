use std::collections::HashMap;

static SAMPLE1: &str = include_str!("../../../../inputs/2023/day1/sample1");
static SAMPLE2: &str = include_str!("../../../../inputs/2023/day1/sample2");
static DATA: &str = include_str!("../../../../inputs/2023/day1/data");

fn part1(input: &str) -> u64 {
    let mut sum: u64 = 0;
    for line in input.split('\n').filter(|&line| !line.is_empty()) {
        let mut digits = Vec::new();
        for c in line.chars() {
            if c.is_ascii_digit() {
                digits.push(c.to_digit(10).unwrap() as u64);
            }
        }
        sum += digits.first().unwrap() * 10 + digits.last().unwrap();
    }

    sum
}

fn part2(input: &str) -> u64 {
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

    let mut sum: u64 = 0;
    for line in input.split('\n').filter(|&line| !line.is_empty()) {
        let mut digits = Vec::new();
        for (i, c) in line.chars().enumerate() {
            if c.is_ascii_digit() {
                digits.push(c.to_digit(10).unwrap() as u64);
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

    sum
}

fn main() {
    dbg!(part1(SAMPLE1));
    dbg!(part1(DATA));
    dbg!(part2(SAMPLE2));
    dbg!(part2(DATA));
}

#[cfg(test)]
mod tests {

    use super::*;
    use test_case::test_case;

    #[test_case(SAMPLE1, 142)]
    #[test_case(DATA, 55488)]
    fn part1_test(input: &str, expected: u64) {
        let actual = part1(input);
        assert_eq!(actual, expected);
    }

    #[test_case(SAMPLE2, 281)]
    #[test_case(DATA, 55614)]
    fn part2_test(input: &str, expected: u64) {
        let actual = part2(input);
        assert_eq!(actual, expected);
    }
}
