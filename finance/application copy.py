from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():

    # get symbols of stocks bought by user.
    stock_symbols = db.execute("SELECT symbol FROM transactions WHERE u_id=:u_id GROUP BY symbol;", u_id=session['user_id'])
    grand_total = 0

    if stock_symbols != []:
        stocks = []
        current_cash = db.execute("SELECT cash FROM users WHERE id = :user_id;", user_id=session['user_id'])


        for symbol in stock_symbols:
            symbol_data = lookup(symbol['symbol'])
            stock_shares = db.execute("SELECT SUM(quantity) FROM transactions WHERE u_id=:u_id AND symbol = :symbol;", u_id=session['user_id'], symbol=symbol_data['symbol'])

            if stock_shares[0]['SUM(quantity)'] == 0:
                continue

        else:
            stock_info = {}

            stock_info['name'] = symbol_data['name']
            stock_info['symbol'] = symbol_data['symbol']
            stock_info['price'] = symbol_data['price']
            stock_info['shares'] = stock_shares[0]['SUM(quantity)']
            stock_info['total'] = stock_info['shares'] * stock_info['price']

        for i in range(len (stocks)):
            grand_total += stocks[i]['total']
        grand_total += current_cash[0]['cash']

        for i in range(len(stocks)):
            stocks[i]['price'] = usd(stocks[i]['price'])
            stocks[i]['total'] = usd(stocks[i]['total'])

        return render_template("index.html", stocks=stocks, current_cash=usd(current_cash[0]['cash']), grand_total= usd(grand_total))

    else:
        current_cash = db.execute("SELECT cash FROM users WHERE id=:user_id", user_id=session['user_id'])
        return render_template("index.html", current_cash=usd(current_cash[0]['cash']), grand_total = usd(current_cash[0]['cash']))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == "POST":
        # check if valid input
        try:
            symbol = lookup(request.form.get("symbol"))
            shares = int(request.form.get("shares"))
        except:
            return apology("enter some input")

        # if symbol is empty return apology
        if not symbol:
            return apology("enter a valid symbol")

        # if shares is empty
        if not shares or shares <= 0:
            return apology("enter the quantity of shares")

        # if can't afford to buy then error
        # get cash from db
        cashOnHand = db.execute("SELECT cash FROM users WHERE id=:user_id;", user_id=session["user_id"])
        cashOnHand = int(cashOnHand[0]['cash'])
        if (shares * symbol['price']) > cashOnHand:
            return apology("can't afford")
        else:
            db.execute("INSERT INTO transactions (symbol, quantity, price, u_id) VALUES (:symbol, :quantity, :price, :u_id);", \
            symbol=symbol['symbol'], quantity=shares, price=symbol['price'], u_id=session["user_id"])
            # update cash (define old_balance)
            db.execute("UPDATE users SET cash=cash-:total_price WHERE id=:user_id;", total_price=shares*symbol['price'], \
            user_id=session["user_id"])
            return redirect(url_for("index"))

    else:
        return render_template("buy.html")



@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    return apology("TODO")

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":

        # use lookup function to get current stock info, given its symbol
        quote = lookup(request.form.get("symbol"))

        # check if quote exists and if it does, display info given by quote list
        if not quote:
            return apology("invalid symbol")

        return render_template("quoted.html", stock=quote)

    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    if request.method =="POST":

    # ENSURE USERNAME WAS SUBMITTED
        if not request.form.get("username"):
            return apology("must provide username")

    #ENSURE PASSWORD WAS SUBMITTED
        elif not request.form.get("password"):
            return apology("must provide password")

    # ENSURE PASSWORD AND PASSWORD CONFIRMATION ARE THE SAME
        elif request.form.get("password") != request.form.get("password-confirm"):
            return apology("Password does not match")

    # INSERT the new user into users, storing a hash of the user’s password
        hash = pwd_context.encrypt(request.form.get("password"))
        result = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)", username=request.form.get("username"), hash=hash)
        if not result:
            return apology("username already exist")

    # LOG USER IN AND REMEMBER LOGIN
        session["user_id"] = result

    # REDIRECT USER TO HOME PAGE
        return redirect(url_for("index"))

    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    if request.method == "POST":
        #CHECK THAT INPUT IS VALID
        try:
            symbol = lookup(request.form.get("symbol"))
            shares = int(request.form.get("shares"))
        except:
            return apology("enter valid input")
        #APOLOGY IF SYMBOL IS INVALID
        if not symbol:
            return apology("enter valid symbol")
            # APOLOGY IF SHARES IS INVALID
        if not shares or shares <= 0:
            return apology("enter valid amount of shares")

        stocks = db.execute("SELECT shares FROM transactions WHERE u_id=u_id AND symbol=:symbol;", u_id=session["user_id"], symbol=quote["symbol"])

        # check that user actually owns enough stock, or any stock at all
        if stocks == []:
            return apology("you don't own this stock")
        if shares > stocks[0]["shares"]:
            return apology("invalid number of shares")

        # enter a new transaction in transactions
        db.execute("INSERT INTO transactions (symbol, quantity, price, u_id) VALUES (:symbol, :quantity, :price, :u_id);", symbol=symbol['symbol'], quantity=-shares, price=symbol['price'], u_id=session["user_id"])


        # UPDATE CASH
        db.execute("UPDATE users SET cash = cash +total_price WHERE id =:user_id;", total_price=shares*symbol['price'], user_id=session["user_id"])

        return redirect(url_for("index"))
    else:
        return render_template("sell.html")


@app.route("/changePassword", methods=["GET", "POST"])
@login_required
def changePassword():
    if request.method == "POST":
        if not request.form.get('password'):
            return apology("must provide old password")
        if not request.form.get("new-password"):
            return apology("must provide new password")
        if not request.form.get("password-confirm"):
            return apology("must provide password confirmation")
        if request.form.get("new-password") != request.form.get("password-confirm"):
            return apology("password must match")

        rows = db.execute("SELECT * FROM users WHERE id = :user_id", user_id=session['user_id'])
        db.execute("UPDATE users SET hash = :hash WHERE id = :user_id", hash = pwd_context.encrypt(request.form.get("new-password")), id = session["user_id"])

        return redirect(url_for("login"))