#include <aoc/message_validator.h>

#include <vector>
#include <variant>
#include <string>
#include <charconv>
#include <cctype>
#include <deque>

using rule_type = std::variant<char,int>;

struct message_validator::impl
{
  std::vector<std::vector<std::vector<rule_type>>> rules;
  std::vector<std::string> messages;
};

message_validator::message_validator(std::istream &setup)
  : impl_(new impl, [](impl const *p) { delete p; })
{
  size_t rule_index{};
  size_t sub_rule_index{};

  while (setup.good()) {
    std::string temp;
    setup >> temp;
    if (temp[temp.size() - 1] == ':') {
      std::from_chars(temp.data(), temp.data() + temp.size() - 1, rule_index);
      if (impl_->rules.size() <= rule_index) {
        impl_->rules.resize(rule_index + 1);
      }
      sub_rule_index = 0;
      impl_->rules[rule_index].emplace_back();

    } else if (std::isdigit(temp[0])) {

      int sub_rule;
      std::from_chars(temp.data(), temp.data() + temp.size(), sub_rule);
      impl_->rules[rule_index][sub_rule_index].emplace_back(sub_rule);

    } else if (temp[0] == '|') {

      ++sub_rule_index;
      impl_->rules[rule_index].emplace_back();

    } else if (temp[0] == '"') {

      impl_->rules[rule_index][sub_rule_index].emplace_back(temp[1]);

    } else {

      impl_->messages.emplace_back(std::move(temp));
    }
  }
}

int message_validator::count_valid() const
{
  auto count = 0;
  auto &rules{ impl_->rules };

  for (auto &message : impl_->messages) {
    std::deque<std::vector<rule_type>> work_queue;

    for (auto &rule : rules[0]) {
      work_queue.emplace_back(rule);
    }

    bool found = false;
    while (!work_queue.empty()
           && !found) {
      auto current{ work_queue.front() };
      work_queue.pop_front();
      auto message_iter = message.begin();

      for (auto c = current.begin(); c != current.end(); ++c) {
        if (c->index() == 0) {
          if (std::get<char>(*c) != *message_iter++) {
            break;
          }
          if (message.end() == message_iter
              && current.end() == c + 1) {
            ++count;
            found = true;
          }
        } else {
          for (auto &sub_rule : rules[std::get<int>(*c)]) {
            auto &added = work_queue.emplace_back(current.begin(), c);
            for (auto &replacement : sub_rule) {
              added.emplace_back(replacement);
            }
            for (auto replacement = c + 1; replacement != current.end(); ++replacement) {
              added.emplace_back(*replacement);
            }
          }
          break;
        }
      }
    }
  }

  return count;
}


