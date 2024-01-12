#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string.h"

constexpr size_t BUFFER_LENGTH = 1024;

enum HandType {
  HIGH_CARD, // 0
  PAIR, // 1
  TWO_PAIRS, // 2
  THREE_OF_A_KIND, // 3
  FULL_HOUSE, // 4
  FOUR_OF_A_KIND, // 5
  FIVE_OF_A_KIND // 6
};

typedef struct {
  enum HandType hand_type;
  char hand[6];
  int bid;
  int jokers;
} Hand;

int cards_ordering[1000];
char cards[] = "AKQJT98765432";
char wildcards[] = "AKQT98765432J";

void init_cards_ordering(bool use_wildcards) {
  size_t cards_size = strlen(cards);
  for (size_t i = 0; i < cards_size; i++) {
    cards_ordering[(unsigned char) (use_wildcards ? wildcards[i] : cards[i])] = i + 1;
  }
}

int card_reverse_cmp(const void *a, const void *b) {
  unsigned char aa = *(unsigned char *) a;
  unsigned char bb = *(unsigned char *) b;
  int cmp = cards_ordering[bb] - cards_ordering[aa];
  //printf("comparing '%c' (%d) and '%c' (%d) = %d\n", aa, cards_ordering[aa], bb, cards_ordering[bb], cmp);
  return cmp;
}

int hand_cmp(const void *a, const void *b) {
  Hand *hand_a = (Hand *) a;
  Hand *hand_b = (Hand *) b;

  //printf("comparing '%s' (%d) and '%s' (%d)\n", hand_a->hand, hand_a->hand_type, hand_b->hand, hand_b->hand_type);
  if (hand_a->hand_type == hand_b->hand_type) {
    for (size_t i = 0; i < 5; i++) {
      int cmp = card_reverse_cmp(&hand_a->hand[i], &hand_b->hand[i]);
      if (cmp != 0) {
        return cmp;
      }
    }
  }
  return hand_a->hand_type - hand_b->hand_type;
}

int int_reverse_cmp(const void *a, const void *b) {
  return *(int *) b - *(int *) a;
}

int part1(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == nullptr) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  int sum = 0;

  size_t hands_size = 0;
  Hand hands[1000];

  char buffer[BUFFER_LENGTH] = {};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';

    char hand_str[6];
    int bid = 0;

    if (sscanf(buffer, "%s %d", hand_str, &bid)) {
      printf("hand = %s, bid = %d\n", hand_str, bid);

      char sorted_hand_str[6];
      strncpy(sorted_hand_str, hand_str, 6);

      qsort(sorted_hand_str, strlen(sorted_hand_str), sizeof(char), card_reverse_cmp);
      printf("sorted hand = %s\n", sorted_hand_str);

      enum HandType hand_type = 0;

      int counts[5] = { 1, 0, 0, 0, 0 };
      size_t count_index = 0;
      for (size_t i = 1; i < 5; i++) {
        char previous_card = sorted_hand_str[i - 1];
        char card = sorted_hand_str[i];
        if (card != previous_card) {
          count_index++;
        }
        counts[count_index]++;
      }
      qsort(counts, 5, sizeof(int), int_reverse_cmp);
      //printf("counts = [ %d, %d, %d, %d, %d ]\n", counts[0], counts[1], counts[2], counts[3], counts[4]);

      if (counts[0] == 5) {
        hand_type = FIVE_OF_A_KIND;
      } else if (counts[0] == 4) {
        hand_type = FOUR_OF_A_KIND;
      } else if (counts[0] == 3 && counts[1] == 2) {
        hand_type = FULL_HOUSE;
      } else if (counts[0] == 3 && counts[1] != 2) {
        hand_type = THREE_OF_A_KIND;
      } else if (counts[0] == 2 && counts[1] == 2) {
        hand_type = TWO_PAIRS;
      } else if (counts[0] == 2 && counts[1] != 2) {
        hand_type = PAIR;
      } else {
        hand_type = HIGH_CARD;
      }
      printf("hand type = %d\n", hand_type);

      Hand hand = {
        .hand_type = hand_type,
        .bid = bid
      };
      strncpy(hand.hand, hand_str, 6);
      hands[hands_size++] = hand;
    }

    printf("%s\n", buffer);
  }

  for (size_t i = 0; i < hands_size; i++) {
    Hand hand = hands[i];
    printf("hand = %s, bid = %d\n", hand.hand, hand.bid);
  }

  qsort(hands, hands_size, sizeof(Hand), hand_cmp);

  printf("sorted hands:\n");

  for (size_t i = 0; i < hands_size; i++) {
    Hand hand = hands[i];
    printf("hand = %s, bid = %d\n", hand.hand, hand.bid);

    sum += hand.bid * (i + 1);
  }

  fclose(fp);

  printf("sum = %d\n", sum);

  return sum;
}

int part2(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == nullptr) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  int sum = 0;

  size_t hands_size = 0;
  Hand hands[1000];

  char buffer[BUFFER_LENGTH] = {};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';

    char hand_str[6];
    int bid = 0;

    if (sscanf(buffer, "%s %d", hand_str, &bid)) {
      printf("hand = %s, bid = %d\n", hand_str, bid);

      char sorted_hand_str[6];
      strncpy(sorted_hand_str, hand_str, 6);

      qsort(sorted_hand_str, strlen(sorted_hand_str), sizeof(char), card_reverse_cmp);
      printf("sorted hand = %s\n", sorted_hand_str);

      enum HandType hand_type = 0;

      int jokers = 0;
      for (size_t i = 0; i < 5; i++) {
        if (hand_str[i] == 'J') {
          jokers++;
        }
      }
      printf("jokers = %d\n", jokers);

      int counts[5] = { 1, 0, 0, 0, 0 };
      size_t count_index = 0;
      for (size_t i = 1; i < 5; i++) {
        char previous_card = sorted_hand_str[i - 1];
        char card = sorted_hand_str[i];
        if (card != previous_card) {
          count_index++;
        }
        counts[count_index]++;
      }
      qsort(counts, 5, sizeof(int), int_reverse_cmp);
      //printf("counts = [ %d, %d, %d, %d, %d ]\n", counts[0], counts[1], counts[2], counts[3], counts[4]);

      if (counts[0] == 5) {
        hand_type = FIVE_OF_A_KIND;
      } else if (counts[0] == 4) {
        if (jokers == 1 || jokers == 4) {
          hand_type = FIVE_OF_A_KIND;
        } else {
          hand_type = FOUR_OF_A_KIND;
        }
      } else if (counts[0] == 3 && counts[1] == 2) {
        if (jokers == 2 || jokers == 3) {
          hand_type = FIVE_OF_A_KIND;
        } else if (jokers == 1) {
          hand_type = FOUR_OF_A_KIND;
        } else {
          hand_type = FULL_HOUSE;
        }
      } else if (counts[0] == 3 && counts[1] != 2) {
        if (jokers == 1 || jokers == 3) {
          hand_type = FOUR_OF_A_KIND;
        } else {
          hand_type = THREE_OF_A_KIND;
        }
      } else if (counts[0] == 2 && counts[1] == 2) {
        if (jokers == 1) {
          hand_type = FULL_HOUSE;
        } else if (jokers == 2) {
          hand_type = FOUR_OF_A_KIND;
        } else {
          hand_type = TWO_PAIRS;
        }
      } else if (counts[0] == 2 && counts[1] != 2) {
        if (jokers == 1 || jokers == 2) {
          hand_type = THREE_OF_A_KIND;
        } else {
          hand_type = PAIR;
        }
      } else {
        if (jokers == 1) {
          hand_type = PAIR;
        } else {
          hand_type = HIGH_CARD;
        }
      }
      printf("hand type = %d\n", hand_type);

      Hand hand = {
        .hand_type = hand_type,
        .bid = bid,
        .jokers = jokers
      };
      strncpy(hand.hand, hand_str, 6),
      hands[hands_size++] = hand;
    }

    printf("%s\n", buffer);
  }

  for (size_t i = 0; i < hands_size; i++) {
    Hand hand = hands[i];
    printf("hand = %s, bid = %d\n", hand.hand, hand.bid);
  }

  qsort(hands, hands_size, sizeof(Hand), hand_cmp);

  printf("sorted hands:\n");

  for (size_t i = 0; i < hands_size; i++) {
    Hand hand = hands[i];
    if (hand.jokers > 0) {
      printf("hand = %s, type = %d, bid = %d\n", hand.hand, hand.hand_type, hand.bid);
    }

    sum += hand.bid * (i + 1);
  }

  fclose(fp);

  printf("sum = %d\n", sum);

  return sum;
}

int main(void) {
  init_cards_ordering(false);

  assert(part1("../../inputs/2023/day7/sample") == 6440);
  assert(part1("../../inputs/2023/day7/data") == 246424613);

  init_cards_ordering(true);

  assert(part2("../../inputs/2023/day7/sample") == 5905);
  assert(part2("../../inputs/2023/day7/data") == 248256639);

  return 0;
}
