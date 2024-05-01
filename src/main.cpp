#include <csignal>
#include <string>

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

#include "muParser.h"

using namespace std;
using namespace ftxui;
using namespace mu;

string calculate(const string& expression) {
  try {
    if (!expression.empty()) {
      Parser parser;
      parser.SetExpr(expression);

      double num = parser.Eval();
      string numString = to_string(num);

      size_t pos = numString.find_last_not_of('0');
      if (pos != std::string::npos && numString[pos] == '.') {
        pos--;
      }

      return numString.substr(0, pos + 1);
    } else {
      return "";
    }
  } catch (Parser::exception_type& e) {
    return e.GetMsg();
  }
}

int main() {
  string expression;
  string calculated = " ";

  auto screen = ScreenInteractive::Fullscreen();

  InputOption expressionsInputOptions = InputOption::Default();
  expressionsInputOptions.on_enter = [&] {
    calculated = calculate(expression);
  };
  expressionsInputOptions.multiline = false;
  Component expressionsInput = Input(&expression, "", expressionsInputOptions);

  Component quitButton = Button(
      "X", [&] { screen.Exit(); }, ButtonOption::Ascii());

  Component clearButton = Button(" C ", [&] {
    expression = "";
    calculated = "";
  });
  Component removeButton = Button(" R ", [&] {
    if (!expression.empty()) expression.pop_back();
  });
  Component plusButton = Button(" + ", [&] { expression.append("+"); });
  Component minusButton = Button(" - ", [&] { expression.append("-"); });
  Component timesButton = Button(" * ", [&] { expression.append("*"); });
  Component dividedByButton = Button(" / ", [&] { expression.append("/"); });
  Component dotButton = Button(" . ", [&] { expression.append("."); });

  Component calculateButton =
      Button(" = ", [&] { calculated = calculate(expression); });

  Component button9 = Button(" 9 ", [&] { expression.append("9"); });
  Component button8 = Button(" 8 ", [&] { expression.append("8"); });
  Component button7 = Button(" 7 ", [&] { expression.append("7"); });
  Component button6 = Button(" 6 ", [&] { expression.append("6"); });
  Component button5 = Button(" 5 ", [&] { expression.append("5"); });
  Component button4 = Button(" 4 ", [&] { expression.append("4"); });
  Component button3 = Button(" 3 ", [&] { expression.append("3"); });
  Component button2 = Button(" 2 ", [&] { expression.append("2"); });
  Component button1 = Button(" 1 ", [&] { expression.append("1"); });
  Component button0 = Button(" 0 ", [&] { expression.append("0"); });

  Component emptyButton2 = Button("   ", [&] {});
  Component emptyButton3 = Button("   ", [&] {});

  auto componentTree = Container::Vertical(
      {quitButton, expressionsInput,
       Container::Horizontal(
           {clearButton, removeButton, emptyButton2, dividedByButton}),
       Container::Horizontal({button7, button8, button9, timesButton}),
       Container::Horizontal({button4, button5, button6, minusButton}),
       Container::Horizontal({button1, button2, button3, plusButton}),
       Container::Horizontal(
           {emptyButton3, button0, dotButton, calculateButton})});

  auto renderer = Renderer(componentTree, [&] {
    return window(
               hbox(text(" "), bold(text("calc++")), text(" "),
                    filler() | borderEmpty, quitButton->Render()),
               vbox(
                   vbox(hbox(
                       hbox(expressionsInput->Render()) | flex | border,
                       hbox(text(" "), text(calculated), text(" ")) | border)),
                   vbox(hbox(clearButton->Render(), removeButton->Render(),
                             emptyButton2->Render(), dividedByButton->Render()),
                        hbox(button7->Render(), button8->Render(),
                             button9->Render(), timesButton->Render()),
                        hbox(button4->Render(), button5->Render(),
                             button6->Render(), minusButton->Render()),
                        hbox(button1->Render(), button2->Render(),
                             button3->Render(), plusButton->Render()),
                        hbox(emptyButton3->Render(), button0->Render(),
                             dotButton->Render(), calculateButton->Render())) |
                       center)) |
           center;
  });

  screen.Loop(renderer);

  return EXIT_SUCCESS;
}
