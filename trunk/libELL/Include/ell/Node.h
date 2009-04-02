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

#ifndef INCLUDED_ELL_NODE_H
#define INCLUDED_ELL_NODE_H

#include <ell/Utils.h>
#include <ell/Storage.h>

namespace ell
{
    template <typename Token>
    struct Parser;

    template <typename Token, typename Child, const int Min, const int Max>
    struct Repeat;

    template <typename Token, typename Child, typename ConcreteParser, typename Var>
    struct Action;

    template <typename Token, typename Left, typename Right>
    struct Alternative;

    template <typename Token, typename Left, typename Right>
    struct Difference;

    template <typename Token, typename Left, typename Right>
    struct Aggregation;

    template <typename Token, typename Left, typename Right>
    struct List;

    template <typename Token, typename Left, typename Right>
    struct BoundRepetition;

    template <typename Token, typename Left, typename Right>
    struct Association;

    template <typename Token>
    struct Node
    {
        Node(const char * name = 0)
          : name(name)
        { }

        virtual ~Node() { }

        virtual bool parse(Parser<Token> *) const = 0;

        friend inline std::ostream & operator << (std::ostream & os, const Node & sr)
        {
            sr.describe(os);
            return os;
        }

#       if ELL_DEBUG == 1
        virtual bool must_be_dumped() const { return ELL_DUMP_NODES; }
#       endif

        void get_value() { }

    protected:
        virtual void describe(std::ostream & os) const = 0;

        const char * name;
    };

    template <typename Token, typename ConcreteNode>
    struct ConcreteNodeBase : public Node<Token>
    {
        bool parse(Parser<Token> * parser) const
        {
            return parse(parser, Storage<void>());
        }

        bool parse(Parser<Token> * parser, Storage<std::basic_string<Token> > & s) const
        {
            const Token * begin = parser->position;
            bool match = ((ConcreteNode *) this)->parse(parser);
            if (match)
                s.value.assign(begin, parser->position);
            return match;
        }

        //@{
        /// Semantic actions
        template <typename ConcreteParser, typename T>
        Action<Token, ConcreteNode, ConcreteParser, T ConcreteParser::*> operator [ ] (T ConcreteParser::* var) const;
        //@}

        //@{
        /// Grammar unary Operator
        Repeat<Token, ConcreteNode, 0, 1> operator ! () const;

        Repeat<Token, ConcreteNode, 1, -1> operator + () const;

        Repeat<Token, ConcreteNode, 0, -1> operator * () const;
        //@}

        //@{
        /// Grammar binary Operator
        template <typename Right>
        Alternative<Token, ConcreteNode, Right> operator | (const Right & r) const;

        template <typename Right>
        Difference<Token, ConcreteNode, Right> operator - (const Right & r) const;

        template <typename Right>
        Aggregation<Token, ConcreteNode, Right> operator >> (const Right & r) const;

        template <typename Right>
        List<Token, ConcreteNode, Right> operator % (const Right & r) const;

        template <typename Right>
        BoundRepetition<Token, ConcreteNode, Right> operator * (const Right & r) const;

        template <typename Right>
        Association<Token, ConcreteNode, Right> operator & (const Right & r) const;
        //@}
    };
}

#include <ell/UnaryNodes.h>
#include <ell/BinaryNodes.h>
#include <ell/Primitives.h>
#include <ell/Numerics.h>

namespace ell
{
    template <typename Token, typename ConcreteNode>
    template <typename ConcreteParser, typename T>
    Action<Token, ConcreteNode, ConcreteParser, T ConcreteParser::*> ConcreteNodeBase<Token, ConcreteNode>::operator [ ](T ConcreteParser::*var) const
    {
        return Action<Token, ConcreteNode, ConcreteParser, T ConcreteParser::*>(* (ConcreteNode *) this, var);
    }

    template <typename Token, typename ConcreteNode>
    Repeat<Token, ConcreteNode, 0, 1> ConcreteNodeBase<Token, ConcreteNode>::operator ! () const
    {
        return Repeat<Token, ConcreteNode, 0, 1>(* (ConcreteNode *) this);
    }

    template <typename Token, typename ConcreteNode>
    Repeat<Token, ConcreteNode, 1, -1> ConcreteNodeBase<Token, ConcreteNode>::operator + () const
    {
        return Repeat<Token, ConcreteNode, 1, -1>(* (ConcreteNode *) this);
    }

    template <typename Token, typename ConcreteNode>
    Repeat<Token, ConcreteNode, 0, -1> ConcreteNodeBase<Token, ConcreteNode>::operator * () const
    {
        return Repeat<Token, ConcreteNode, 0, -1>(* (ConcreteNode *) this);
    }

    template <typename Token, typename ConcreteNode>
    template <typename Right>
    Alternative<Token, ConcreteNode, Right> ConcreteNodeBase<Token, ConcreteNode>::operator | (const Right & r) const
    {
        return Alternative<Token, ConcreteNode, Right>(* (ConcreteNode *) this, r);
    }

    template <typename Token, typename ConcreteNode>
    template <typename Right>
    Difference<Token, ConcreteNode, Right> ConcreteNodeBase<Token, ConcreteNode>::operator - (const Right & r) const
    {
        return Difference<Token, ConcreteNode, Right>(* (ConcreteNode *) this, r);
    }

    template <typename Token, typename ConcreteNode>
    template <typename Right>
    Aggregation<Token, ConcreteNode, Right> ConcreteNodeBase<Token, ConcreteNode>::operator >> (const Right & r) const
    {
        return Aggregation<Token, ConcreteNode, Right>(* (ConcreteNode *) this, r);
    }

    template <typename Token, typename ConcreteNode>
    template <typename Right>
    List<Token, ConcreteNode, Right> ConcreteNodeBase<Token, ConcreteNode>::operator % (const Right & r) const
    {
        return List<Token, ConcreteNode, Right>(* (ConcreteNode *) this, r);
    }

    template <typename Token, typename ConcreteNode>
    template <typename Right>
    BoundRepetition<Token, ConcreteNode, Right> ConcreteNodeBase<Token, ConcreteNode>::operator * (const Right & r) const
    {
        return BoundRepetition<Token, ConcreteNode, Right>(* (ConcreteNode *) this, r);
    }

    template <typename Token, typename ConcreteNode>
    template <typename Right>
    Association<Token, ConcreteNode, Right> ConcreteNodeBase<Token, ConcreteNode>::operator & (const Right & r) const
    {
        return Association<Token, ConcreteNode, Right>(* (ConcreteNode *) this, r);
    }
}

#endif // INCLUDED_ELL_NODE_H
