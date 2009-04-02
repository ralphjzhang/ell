// This file is part of Ell library.
//
// Ell library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Ell library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Ell library.  If not, see <http://www.gnu.org/licenses/>.

#ifndef INCLUDED_ELL_BINARY_NODE_H
#define INCLUDED_ELL_BINARY_NODE_H

#include <ell/Rule.h>

namespace ell
{
    template <typename Token, typename ConcreteNode, typename BinaryFlavour>
    struct BinaryNodeBase : public ConcreteNodeBase<Token, ConcreteNode>
    {
        void describe(std::ostream & os) const
        {
            os << '(' << ((BinaryFlavour *) this)->left
               << ' ' << ConcreteNodeBase<Token, ConcreteNode>::name
               << ' ' << ((BinaryFlavour *) this)->right << ')';
        }
    };

    template <typename Token, typename ConcreteNode, typename TLeft, typename TRight>
    struct BinaryNode : public BinaryNodeBase<Token, ConcreteNode, BinaryNode<Token, ConcreteNode, TLeft, TRight> >
    {
        BinaryNode(const TLeft & left, const TRight & right, const char * n)
          : left(left), right(right)
        {
            Node<Token>::name = n;
        }

        TLeft left;
        TRight right;
    };

    template <typename Token, typename ConcreteNode, typename TLeft>
    struct BinaryNode<Token, ConcreteNode, TLeft, Rule<Token> >
      : public BinaryNodeBase<Token, ConcreteNode, BinaryNode<Token, ConcreteNode, TLeft, Rule<Token> > >
    {
        BinaryNode(const TLeft & left, const Rule<Token> & right, const char * n)
          : left(left), right(right)
        {
            Node<Token>::name = n;
        }

        TLeft left;
        const Rule<Token> & right;
    };

    template <typename Token, typename ConcreteNode, typename TRight>
    struct BinaryNode<Token, ConcreteNode, Rule<Token>, TRight>
      : public BinaryNodeBase<Token, ConcreteNode, BinaryNode<Token, ConcreteNode, Rule<Token>, TRight> >
    {
        BinaryNode(const Rule<Token> & left, const TRight & right, const char * n)
          : left(left), right(right)
        {
            Node<Token>::name = n;
        }

        const Rule<Token> & left;
        TRight right;
    };

    template <typename Token, typename ConcreteNode>
    struct BinaryNode<Token, ConcreteNode, Rule<Token>, Rule<Token> >
      : public BinaryNodeBase<Token, ConcreteNode, BinaryNode<Token, ConcreteNode, Rule<Token>, Rule<Token> > >
    {
        BinaryNode(const Rule<Token> & left, const Rule<Token> & right, const char * n)
          : left(left), right(right)
        {
            Node<Token>::name = n;
        }

        const Rule<Token> & left;
        const Rule<Token> & right;
    };
}

#endif // INCLUDED_ELL_BINARY_NODE_H
