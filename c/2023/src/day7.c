#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commons.h"
#include "string.h"

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
  //debug("comparing '%c' (%d) and '%c' (%d) = %d\n", aa, cards_ordering[aa], bb, cards_ordering[bb], cmp);
  return cmp;
}

int hand_cmp(const void *a, const void *b) {
  Hand *ha = (Hand *) a;
  Hand *hb = (Hand *) b;

  //debug("comparing '%s' (%d) and '%s' (%d)\n", hand_a->hand, hand_a->hand_type, hand_b->hand, hand_b->hand_type);
  if (ha->hand_type == hb->hand_type) {
    for (size_t i = 0; i < 5; i++) {
      int cmp = card_reverse_cmp(&ha->hand[i], &hb->hand[i]);
      if (cmp != 0) {
        return cmp;
      }
    }
  }
  return ha->hand_type - hb->hand_type;
}

int int_reverse_cmp(const void *a, const void *b) {
  return *(int *) b - *(int *) a;
}

int part1(StringArray *lines) {
  int result = 0;

  size_t hands_size = 0;
  Hand hands[1000];

  string_array_foreach (char *line, lines) {
    char hand_str[6];
    int bid = 0;

    if (sscanf(line, "%s %d", hand_str, &bid)) {
      debug("hand = %s, bid = %d\n", hand_str, bid);

      char sorted_hand_str[6];
      strncpy(sorted_hand_str, hand_str, 6);

      qsort(sorted_hand_str, strlen(sorted_hand_str), sizeof(char), card_reverse_cmp);
      debug("sorted hand = %s\n", sorted_hand_str);

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
      debug("counts = [ %d, %d, %d, %d, %d ]\n", counts[0], counts[1], counts[2], counts[3], counts[4]);

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
      debug("hand type = %d\n", hand_type);

      Hand hand = {
        .hand_type = hand_type,
        .bid = bid
      };
      strncpy(hand.hand, hand_str, 6);
      hands[hands_size++] = hand;
    }

    debug("%s\n", line);
  }

#ifdef DEBUG
  for (size_t i = 0; i < hands_size; i++) {
    Hand hand = hands[i];
    debug("hand = %s, bid = %d\n", hand.hand, hand.bid);
  }
#endif

  qsort(hands, hands_size, sizeof(Hand), hand_cmp);

  debug("sorted hands:\n");

  for (size_t i = 0; i < hands_size; i++) {
    Hand hand = hands[i];
    debug("hand = %s, bid = %d\n", hand.hand, hand.bid);

    result += hand.bid * (i + 1);
  }

  return result;
}

int part2(StringArray *lines) {
  int result = 0;

  size_t hands_size = 0;
  Hand hands[1000];

  string_array_foreach (char *line, lines) {
    char hand_str[6];
    int bid = 0;

    if (sscanf(line, "%s %d", hand_str, &bid)) {
      debug("hand = %s, bid = %d\n", hand_str, bid);

      char sorted_hand_str[6];
      strncpy(sorted_hand_str, hand_str, 6);

      qsort(sorted_hand_str, strlen(sorted_hand_str), sizeof(char), card_reverse_cmp);
      debug("sorted hand = %s\n", sorted_hand_str);

      enum HandType hand_type = 0;

      int jokers = 0;
      for (size_t i = 0; i < 5; i++) {
        if (hand_str[i] == 'J') {
          jokers++;
        }
      }
      debug("jokers = %d\n", jokers);

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
      debug("counts = [ %d, %d, %d, %d, %d ]\n", counts[0], counts[1], counts[2], counts[3], counts[4]);

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
      debug("hand type = %d\n", hand_type);

      Hand hand = {
        .hand_type = hand_type,
        .bid = bid,
        .jokers = jokers
      };
      strncpy(hand.hand, hand_str, 6),
      hands[hands_size++] = hand;
    }

    debug("%s\n", line);
  }

#ifdef DEBUG
  for (size_t i = 0; i < hands_size; i++) {
    Hand hand = hands[i];
    debug("hand = %s, bid = %d\n", hand.hand, hand.bid);
  }
#endif

  qsort(hands, hands_size, sizeof(Hand), hand_cmp);

  debug("sorted hands:\n");

  for (size_t i = 0; i < hands_size; i++) {
    Hand hand = hands[i];
    if (hand.jokers > 0) {
      debug("hand = %s, type = %d, bid = %d\n", hand.hand, hand.hand_type, hand.bid);
    }

    result += hand.bid * (i + 1);
  }

  return result;
}

int main(void) {
  init_cards_ordering(false);

  test_case(day7, part1, sample, 6440);
  test_case(day7, part1, data, 246424613);

  init_cards_ordering(true);

  test_case(day7, part2, sample, 5905);
  test_case(day7, part2, data, 248256639);

  return 0;
}
