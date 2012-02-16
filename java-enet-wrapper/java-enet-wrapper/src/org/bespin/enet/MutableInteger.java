package org.bespin.enet;

public class MutableInteger extends Number
{
    private int value;
    
    public MutableInteger(int value)
    {
        this.value = value;
    }
    
    public void setValue(int newValue)
    {
        this.value = newValue;
    }

    @Override
    public double doubleValue()
    {
        return (double) this.value;
    }

    @Override
    public float floatValue()
    {
        return (float) this.value;
    }

    @Override
    public int intValue()
    {
        return this.value;
    }

    @Override
    public long longValue()
    {
        return this.value;
    }
}
