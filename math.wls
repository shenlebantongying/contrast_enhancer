#!/usr/bin/env wolframscript
(* ::Package:: *)

Plot[Piecewise[{{x/12.92,x<=0.04050},{((x+0.055)/1.055)^2.4,x>0.04045}}],{x,0,1}]
