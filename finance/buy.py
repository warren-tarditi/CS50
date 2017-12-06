if request.method == "POST":

        # CHECK IF VALID INPUT
        try:
            symbol = lookup(request.form.get("symbol"))
            shares = int(request.form.get("shares"))
        except:
            return apology("enter valid input")

        # CHECK IF SYMBOL ENTERED IS VALID IF NOT RETURN APOLOGY
        if not symbol:
            return apology("enter valid symbol")
        # CHECK THAT VALID NUMBER OF SHARES IS ENTERED
        if shares < 1:
            return apology("enter valid number of shares")

        # compare user's cash amount to calculated cost of shares
        id = session["user_id"]
        sharePrice = int(request.fom.get("shares")) * symbol.get("price")
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id = id)

        # IF NOT ENOUGH CASH RETURN APOLOGY
        if sharePrice > cash[0] ["cash"]:
            return apology("insufficient funds")

        transactions = cash - shareprice
        command = """INSERT INTO transactions(user_id, symbol, name, share, price,) VALUES (:user_id, :symbol, :name, :share, :price)"""

        db.execute(command, user_id=id, symbol=symbol.get('symbol'), name = symbol.get('name'), share = int(request.form.get("shares")), price = symbol.get('price'))
        db.execute("UPDATE users SET cash = :cash WHERE id= :id", cash = transactions, id=id)

        return redirect(url_for("index"))

    else:
        render_template("buy.html")