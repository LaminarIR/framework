/*
 * Copyright 2003 by the Massachusetts Institute of Technology.
 *
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in
 * advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.
 * M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 */

/* Code related to LaminarIR:
 * See file LICENSE.LaminarIR which has been distributed with this file.
 */

package laminarir.frontend.nodes;

/**
 * A primitive type.  This can be int, float, or complex, depending on
 * the specified type parameter.
 *
 * @author  David Maze &lt;dmaze@cag.lcs.mit.edu&gt;
 * @version $Id: TypePrimitive.java,v 1.14 2006-01-25 17:04:25 thies Exp $
 */
public class TypePrimitive extends Type
{
    /** Type constant for bit types. */
    public static final int TYPE_BIT = 1;
    /** Type constant for int types. */
    public static final int TYPE_INT = 2;
    /** Type constant for float types. */
    public static final int TYPE_FLOAT = 3;
    /** Type constant for double types; unused in StreamIt. */
    public static final int TYPE_DOUBLE = 4;
    /** Type constant for complex primitive types. */
    public static final int TYPE_COMPLEX = 5;
    /** Type constant for void types. */
    public static final int TYPE_VOID = 6;
    /** Type constant for boolean types. */
    public static final int TYPE_BOOLEAN = 7;
    /** Type constant for char types. */
    public static final int TYPE_CHAR = 8;
    /** Type constant for float vector. */
    public static final int TYPE_FLOAT2 = 9;
    /** Type constant for float vector. */
    public static final int TYPE_FLOAT3 = 10;
    /** Type constant for float vector. */
    public static final int TYPE_FLOAT4 = 11;
    /** Type constant for string types. */
    public static final int TYPE_STRING = 0;

    /** Type object for boolean types. */
    public static final TypePrimitive booltype =
        new TypePrimitive(TYPE_BOOLEAN);
    /** Type object for bit types. */
    public static final TypePrimitive bittype = new TypePrimitive(TYPE_BIT);
    /** Type object for int types. */
    public static final TypePrimitive inttype = new TypePrimitive(TYPE_INT);
    /** Type object for float types. */
    public static final TypePrimitive floattype =
        new TypePrimitive(TYPE_FLOAT);
    /** Type object for complex primitive types. */
    public static final TypePrimitive cplxtype =
        new TypePrimitive(TYPE_COMPLEX);
    /** Type object for void types. */
    public static final TypePrimitive voidtype =
        new TypePrimitive(TYPE_VOID);
    /** Type object for char types. */
    public static final TypePrimitive chartype =
        new TypePrimitive(TYPE_CHAR);
    /** Type object for string types. */
    public static final TypePrimitive stringtype =
        new TypePrimitive(TYPE_STRING);
    
    private int type;

    /**
     * Create a new primitive type.
     *
     * @param type  integer type number, one of the TYPE_* constants
     */
    public TypePrimitive(int type)
    {
        this.type = type;
    }
    
    /**
     * Get the type number for this type.
     *
     * @return  integer type number, one of the TYPE_* constants
     */
    public int getType()
    {
        return type;
    }

    public boolean isComplex()
    {
        return type == TYPE_COMPLEX;
    }

    public boolean isComposite() {
        return type == TYPE_FLOAT2 ||
            type == TYPE_FLOAT3 ||
            type == TYPE_FLOAT4;
    }

    public String toString()
    {
        switch (type)
            {
            case TYPE_BIT:
                return "bit";
            case TYPE_INT:
                return "int";
            case TYPE_FLOAT:
                return "float";
            case TYPE_DOUBLE:
                return "double";
            case TYPE_COMPLEX:
                return "complex";
            case TYPE_VOID:
                return "void";
            case TYPE_BOOLEAN:
                return "boolean";
            case TYPE_CHAR:
                return "char";
            case TYPE_STRING:
                return "string";
            default:
                return "<primitive type " + type + ">";
            }
    }
    
    /**
     * Check if this type can be promoted to some other type.
     * Returns true if a value of this type can be assigned to
     * a variable of that type.  For primitive types, promotions
     * are ordered: boolean -> bit -> int -> float -> complex.
     *
     * @param that  other type to check promotion to
     * @return      true if this can be promoted to that
     */
    public boolean promotesTo(Type that)
    {
        if (super.promotesTo(that))
            return true;
        if (!(that instanceof TypePrimitive))
            return false;

        int t1 = this.type;
        int t2 = ((TypePrimitive)that).type;
        
        // want: "t1 < t2", more or less
        switch(t1)
            {
            case TYPE_BOOLEAN:
                return t2 == TYPE_BOOLEAN || t2 == TYPE_BIT ||
                    t2 == TYPE_INT || t2 == TYPE_FLOAT ||
                    t2 == TYPE_COMPLEX || t2 == TYPE_STRING;
            case TYPE_BIT:
                return t2 == TYPE_BIT || t2 == TYPE_INT ||
                    t2 == TYPE_FLOAT || t2 == TYPE_COMPLEX ||
                    t2 == TYPE_STRING;
            case TYPE_INT:
                return t2 == TYPE_INT || t2 == TYPE_FLOAT || t2 == TYPE_COMPLEX ||
                    t2 == TYPE_STRING;
            case TYPE_FLOAT:
                return t2 == TYPE_FLOAT || t2 == TYPE_COMPLEX ||
                    t2 == TYPE_STRING || t2 == TYPE_FLOAT2 ||
                    t2 == TYPE_FLOAT3 || t2 == TYPE_FLOAT4;     
            case TYPE_FLOAT2:
            case TYPE_FLOAT3:
            case TYPE_FLOAT4:
                return t2 == t1; 
            case TYPE_COMPLEX:
                return t2 == TYPE_COMPLEX || t2 == TYPE_STRING;
            case TYPE_CHAR:
                return t2 == TYPE_CHAR || t2 == TYPE_STRING;
            case TYPE_STRING:
                return t2 == TYPE_STRING;
            default:
                assert false : t1;
                return false;
            }
    }

    public boolean equals(Object other)
    {
        // Two cases.  One, this is complex, and so is that:
        if (other instanceof Type)
            {
                Type that = (Type)other;
                if (this.isComplex() && that.isComplex())
                    return true;
            }
        // Two, these are both primitive types with the same type code.
        if (!(other instanceof TypePrimitive))
            return false;
        TypePrimitive that = (TypePrimitive)other;
        if (this.type != that.type)
            return false;
        return true;
    }
    
    public int hashCode()
    {
        return new Integer(type).hashCode();
    }
}
