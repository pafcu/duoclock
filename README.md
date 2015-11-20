# duoclock
Pebble watchface that shows current time as text and a random word from Duolingo.

To add a new language:
1. Increase LANG_COUNT in languages.h
2. Extend the lang_t enum in languages.h
3. Append the necessary information to the languages[] array in languages.h. NOTE: Languages MUST be in the same order as in the lang_t enum!
4. Add number creation logic to make_numbers() in languages.h
5. Edit settings_N.html in the gh-pages branch, and add another option corresponing to the new language. NOTE: The "value" attribute MUST be the same as in the lang_t enum!
6. Add the language code to lang_codes in duo.js
