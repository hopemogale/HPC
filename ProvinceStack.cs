using System;
using System.Collections.Generic;

public class Provinces
{
    public static void Main()
    {
        Stack<string> numbers = new Stack<string>();
        numbers.Push("GP");
        numbers.Push("NC");
        numbers.Push("WC");
        numbers.Push("EC");
        numbers.Push("KZN");

        // A stack can be enumerated without disturbing its contents.
        foreach( string number in numbers )
        {
            Console.WriteLine(number);
        }

        Console.WriteLine("\nPopping '{0}'", numbers.Pop());
        Console.WriteLine("Peek at next item to destack: {0}",
            numbers.Peek());
        Console.WriteLine("Popping '{0}'", numbers.Pop());

    }
}

