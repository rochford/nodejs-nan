/*
 * Copyright 2015-2016, Tim Rochford
 */

"use_strict";

var hello = require('bindings')('hello');

function callback(options, result)
{
    console.log(options);
    console.log(result);
}

hello.process(callback, {opt1: 'aa', opt2: 'bb'});

