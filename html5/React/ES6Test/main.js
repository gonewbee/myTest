// https://www.twilio.com/blog/2015/08/setting-up-react-for-es6-with-webpack-and-babel-2.html
import Hello from './hello.jsx';
import World from './world.jsx';
import React from 'react';
import ReactDOM from 'react-dom';
 
ReactDOM.render(<Hello/>, document.getElementById('hello'));
ReactDOM.render(<World/>, document.getElementById('world'));
