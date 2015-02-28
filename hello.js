/*
 * Copyright 2015, Tim Rochford
 */

"use_strict";

var hello = require('./build/Release/hello.node');

function callback(options, result)
{
    console.log(options);
    console.log(result);
}

hello.process(callback, {opt1: 'aa', opt2: 'bb'});

