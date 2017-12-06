import cs50

def main():
    while True:
        print("O hai! How much change is owed?")
        amount = cs50.get_float()
        if amount >= 0:
            break

    cents = int(100 * amount)
    print (change(cents))

def change(cents):
    return cents//25 + (cents%25)//10 + (cents%25%10)//5 + cents%25%10%5

if __name__ == "__main__":
    main()
