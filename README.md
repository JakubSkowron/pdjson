# Public Domain JSON Parser and Formatter for C++

This is a work in progress, it does not work yet.

A public domain JSON parser focused on correctness,
compliance, full Unicode (UTF-8) support, minimal memory footprint,
and a simple API. As a streaming API, arbitrary large JSON could be
processed with a small amount of memory (the size of the largest
string in the JSON). It seems most C JSON libraries suck in some
significant way: broken string support (what if the string contains
`\u0000`?), broken/missing Unicode support, or crappy software license
(GPL or "do no evil"). This library intends to avoid these flaws.

The parser is intended to support *exactly* the JSON standard, no
more, no less, so that even slightly non-conforming JSON is rejected.
The input is assumed to be UTF-8, and all strings returned by the
library are UTF-8 with possible nul characters in the middle, which is
why the size output parameter is important. Encoded characters
(`\uxxxx`) are decoded and re-encoded into UTF-8. UTF-16 surrogate
pairs expressed as adjacent encoded characters are supported.

One exception to this rule is made to support a "streaming" mode. When
a JSON "stream" contains multiple JSON objects (optionally separated
by JSON whitespace), the default behavior of the parser is to allow
the stream to be "reset," and to continue parsing the stream.

## API Overview

### Printer API
Example usage (printing JSON):

```c++
  pdjson::printer json;

  json.new_object();
    json.number( "a", -0.2 );
    json.number( "b", 100 );
    json.new_object( u8"😍" );
      json.string( u8"chrząszcz", u8"[ˈxʂɔw̃ʂt͡ʂ]" );
      json.new_array("a");
        json.number(1);
        json.number(2);
        json.number(3);
        json.new_array();
        json.end_array();
      json.end_array();
    json.end_object();
  json.end_object();
```

Get result by calling `json.str()`

```json
{"a":-0.2,"b":100,"😍":{"chrząszcz":"[ˈxʂɔw̃ʂt͡ʂ]","a":[1,2,3,[]]}}
```
