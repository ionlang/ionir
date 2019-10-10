#pragma once

#include "generation/expr.hpp"
#include "generation/expr_type.hpp"
#include "expression/prototype.hpp"

class Extern : public Expr
{
private:
	Prototype prototype;

public:
	Extern(Prototype prototype) : Expr(ExprType::Extern), prototype(prototype)
	{
		this->prototype = prototype;
	}

	Prototype getPrototype()
	{
		return this->prototype;
	}
};