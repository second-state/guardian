//pragma solidity ^0.4.24;
import "A.sol" as SS;
contract baseContract is SS.String,SS.AA{
	string GG;
	struct P{
		int a;
	}
	P c;
	SS.String sss;
	bytes5 bb;
	bytes bs;
	enum ActionChoices { GoLeft, GoRight, GoStraight, SitStill }
    ActionChoices choice;
	ufixed fff;
	SS.AA.PPP pppppp;
	mapping(address => uint) public balances;
	function set()public{
		int b;
		GG = "\n	\"";
	}
}