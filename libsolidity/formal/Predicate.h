/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/
// SPDX-License-Identifier: GPL-3.0

#pragma once

#include <libsolidity/formal/SymbolicVariables.h>
#include <libsolidity/formal/SymbolicVariables.h>

#include <libsmtutil/Sorts.h>

#include <map>
#include <optional>
#include <vector>

namespace solidity::frontend
{

/**
 * Represents a predicate used by the CHC engine.
 */
class Predicate
{
public:
	static Predicate const* create(
		smtutil::SortPointer _sort,
		std::string _name,
		smt::EncodingContext& _context,
		ASTNode const* _node = nullptr
	);

	static Predicate const* predicate(std::string const& _name);

	static void reset();

	/// @returns a function application of the predicate over _args.
	smtutil::Expression operator()(std::vector<smtutil::Expression> const& _args) const;

	/// @returns the function declaration of the predicate.
	smtutil::Expression functor() const;
	/// @returns the function declaration of the predicate with index _idx.
	smtutil::Expression functor(unsigned _idx) const;
	/// Increases the index of the function declaration of the predicate.
	void newFunctor();

	/// @returns the program node this predicate represents.
	ASTNode const* programNode() const;

	/// @returns the ContractDefinition that this predicate represents
	/// or nullptr otherwise.
	ContractDefinition const* programContract() const;

	/// @returns the FunctionDefinition that this predicate represents
	/// or nullptr otherwise.
	FunctionDefinition const* programFunction() const;

	/// @returns the program state variables in the scope of this predicate.
	std::optional<std::vector<VariableDeclaration const*>> stateVariables() const;

	/// @returns true if this predicate represents a summary.
	bool isSummary() const;

	/// @returns true if this predicate represents an interface.
	bool isInterface() const;

	/// @returns a formatted string representing a call to this predicate
	/// with _args.
	std::string formatSummaryCall(std::vector<std::string> const& _args) const;

	/// @returns the values of the state variables from _args at the point
	/// where this summary was reached.
	std::vector<std::string> summaryStateValues(std::vector<std::string> const& _args) const;

private:
	Predicate(
		smtutil::SortPointer _sort,
		std::string _name,
		smt::EncodingContext& _context,
		ASTNode const* _node = nullptr
	);

	/// The actual SMT expression.
	smt::SymbolicFunctionVariable m_predicate;

	/// The ASTNode that this predicate represents.
	/// nullptr if this predicate is not associated with a specific program AST node.
	ASTNode const* m_node = nullptr;

	/// Maps the name of the predicate to the actual Predicate.
	/// Used in counterexample generation.
	static std::map<std::string, Predicate> m_predicates;
};

}
