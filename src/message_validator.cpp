#include <aoc/message_validator.h>

#include <vector>
#include <variant>
#include <string>
#include <charconv>
#include <cctype>
#include <deque>
#include <iostream>
#include <algorithm>

using rule_type = std::variant<char,int>;

bool rule_in_use(const std::vector<std::vector<std::vector<rule_type>>> &rules, int rule_number)
{
  if (rule_number == 0) return true;
  for (auto& rule : rules) {
    for(auto& sub_rule: rule) {
      for(auto& c: sub_rule) {
        if (c.index() == 1 && std::get<int>(c) == rule_number) {
          return true;
        }
      }
    }
  }

  return false;
}

void print_rules(const std::vector<std::vector<std::vector<rule_type>>>& rules)
{
  for (int i = 0; i < rules.size(); ++i) {
    if (!rules[i].empty()) {
      if (rule_in_use(rules, i)) {
        std::cout << "*";
      }
      else {
        std::cout << " ";
      }
      std::cout << i << ": ";
      auto first_sub_rule = true;
      for (auto& sub_rule: rules[i]) {
        if (!first_sub_rule) {
          std::cout << "| ";
        } else {
          first_sub_rule = false;
        }
        for(auto& c: sub_rule) {
          if (c.index() == 0)
            std::cout << '"' << std::get<char>(c) << '"';
          else
            std::cout << std::get<int>(c);
          std::cout << " ";
        }
      }
      std::cout << "\n";
    }
  }
}

struct message_validator::impl
{
  std::vector<std::vector<std::vector<rule_type>>> rules;
  std::vector<std::string> messages;

  void optimize_rules();
};

void message_validator::impl::optimize_rules()
{
  bool replaced;
  auto replaced_count = 0;
  do {
    replaced = false;
    auto rule_index = 0;
    for (auto &rule : rules) {
      auto sub_rule_index = 0;
      for (auto &sub_rule : rule) {
        auto value_index = 0;
        for (auto value = sub_rule.begin(); value != sub_rule.end(); ++value) {
          if (value->index() == 1) {
            auto &lookup = rules[std::get<int>(*value)];
            if (lookup.size() == 1) {
              ++replaced_count;
              std::vector<rule_type> replacement_sub_rule(sub_rule.begin(), value);
              for (auto &replacement_value : lookup[0]) {
                replacement_sub_rule.emplace_back(replacement_value);
              }
              for (auto replacement_value = value + 1; replacement_value != sub_rule.end(); ++replacement_value) {
                replacement_sub_rule.emplace_back(*replacement_value);
              }
              sub_rule = replacement_sub_rule;
              replaced = true;
              break;
            }
          }
          ++value_index;
        }
        ++sub_rule_index;
      }
      ++rule_index;
    }
  } while (replaced);
  
}


message_validator::message_validator(std::istream &setup, flag<option> options /*= option::none*/)
  : options_(options), impl_(new impl, [](impl const *p) { delete p; })
{
  size_t rule_index{};
  size_t sub_rule_index{};

  //std::istream *input = &setup;

  while (setup.good()) {
    std::string temp;
    setup >> temp;
    if (options_.has(option::day2)) {
    }
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
  if (options_.has(option::print_rules)) {
    print_rules(impl_->rules);
  }
}



int message_validator::count_valid() const
{
  if (options_.has(option::optimize_rules)) {
    impl_->optimize_rules();
    if (options_.has(option::print_rules)) {
      print_rules(impl_->rules);
    }
  }

  auto count = 0;
  auto &rules{ impl_->rules };
  auto total_iterations = 0;

  for (auto &message : impl_->messages) {
    std::deque<std::vector<rule_type>> work_queue;

    for (auto &rule : rules[0]) {
      work_queue.emplace_back(rule);
    }

    auto iterations = 0;
    size_t max_queue_count = 0;

    bool found = false;
    while (!work_queue.empty()
           && !found) {
      ++iterations;
      max_queue_count = std::max(max_queue_count, work_queue.size());
      auto current{ work_queue.front() };
      work_queue.pop_front();
      auto message_iter = message.begin();

      if (current.size() > message.size()) continue;

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
    total_iterations += iterations;
    if (options_.has(option::print_stats)) {
      std::cout << message << " found=" << found << " iterations=" << iterations << " max_queue=" << max_queue_count << "\n";
    }
  }
  if (options_.has(option::print_stats)) {
    std::cout << "total_iterations=" << total_iterations << "\n";
  }

  return count;
}


