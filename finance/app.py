import os
from datetime import datetime

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    userInfo = []
    stockTotal = 0
    cash = db.execute("SELECT cash FROM users WHERE id=?", session.get("user_id"))[0]["cash"]
    stocks = db.execute(
        "SELECT symbol FROM purchases WHERE user_id=? GROUP BY symbol HAVING type='buy';", session.get("user_id"))
    for stock in stocks:
        SYMBOL = stock["symbol"]
        boughtShare = db.execute(
            "SELECT SUM(share) AS s FROM purchases WHERE symbol=? AND type='buy' AND user_id=?", SYMBOL, session.get("user_id"))[0]["s"]
        sold = db.execute("SELECT SUM(share) AS s FROM purchases WHERE symbol=? AND type='sell' AND user_id=?",
                          SYMBOL, session.get("user_id"))[0]["s"]
        soldShare = 0
        if sold != None:
            soldShare = int(sold)
        else:
            soldShare = 0
        if boughtShare != None:
            boughtShare = int(boughtShare)
        else:
            boughtShare = 0
        SHARE = boughtShare - soldShare
        PRICE = float(lookup(SYMBOL)["price"])
        stockTotal += PRICE * SHARE
        if SHARE > 0:
            userInfo.append({
                "symbol": SYMBOL,
                "name": lookup(SYMBOL)["name"],
                "share": SHARE,
                "price": usd(PRICE),
                "total": usd(PRICE * SHARE)
            })
    total = cash + stockTotal
    return render_template("index.html", userInfo=userInfo, cash=usd(cash), total=usd(total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        currentUser = session.get("user_id")
        symbol = request.form.get("symbol")
        if symbol.isalpha():
            symbol = symbol.upper()
        else:
            return apology("Invalid Character!")
        shares = request.form.get("shares")
        if lookup(symbol) == None:
            return apology("Invalid symbol")

        if shares.isdigit() == False:
            return apology("Enter a positive Integer")
        if float(shares).is_integer() == False or float(shares) < 0:
            return apology("Enter a positive Integer")

        shares = int(shares)
        time = datetime.now()
        price = float(lookup(symbol)["price"])

        currentBalance = db.execute("SELECT cash FROM users WHERE id=?", currentUser)[0]["cash"]
        total = price * shares
        if currentBalance < total:
            return apology("Not enough money!")

        db.execute("UPDATE users SET cash=? WHERE id=?", currentBalance - total, currentUser)
        db.execute("INSERT INTO purchases (user_id, symbol, share, time, price, type) VALUES (?, ?, ?, ?, ?, ?)",
                   currentUser, symbol, shares, time, price, "buy")
        return redirect("/")
    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    history = db.execute("SELECT * FROM purchases WHERE user_id=?;", session.get("user_id"))
    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol").upper()
        search = lookup(symbol)
        if search == None:
            return apology("Does not exist")
        return render_template("quoted.html", name=search["name"], price=str(search["price"]), symbol=search["symbol"])
    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        if username == "" or password == "" or confirmation == "":
            return apology("All fields required")
        elif password != confirmation:
            return apology("Passwords does not match")
        elif len(db.execute("SELECT * FROM users WHERE username=?", username)) != 0:
            return apology("User name taken")
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, generate_password_hash(password))
        session["user_id"] = db.execute("SELECT id FROM users WHERE username=?", username)
        return redirect("/login")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    haveStocks = db.execute("SELECT symbol FROM purchases WHERE user_id=? GROUP BY symbol;", session.get("user_id"))
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))
        price = lookup(symbol)["price"]
        boughtShare = db.execute("SELECT SUM(share) AS s FROM purchases WHERE symbol=? AND type='buy'", symbol)[0]["s"]
        soldShare = db.execute("SELECT SUM(share) AS s FROM purchases WHERE symbol=? AND type='sell'", symbol)[0]["s"]
        if boughtShare != None:
            boughtShare = int(boughtShare)
        else:
            boughtShare = 0
        if soldShare != None:
            soldShare = int(soldShare)
        else:
            soldShare = 0
        haveShare = boughtShare - soldShare
        if symbol == None or shares == None:
            return apology("Type all the fields in!")
        if haveShare < shares:
            return apology("You don't have it!")
        db.execute("INSERT INTO purchases (user_id, symbol, share, time, price, type) VALUES (?, ?, ?, ?, ?, ?)",
                   session.get("user_id"), symbol, shares, datetime.now(), price, "sell")
        currentCash = db.execute("SELECT cash FROM users WHERE id=?", session.get("user_id"))[0]["cash"]
        db.execute("UPDATE users SET cash=? WHERE id=?", currentCash + (shares * price), session.get("user_id"))
        return redirect("/")
    return render_template("sell.html", haveStocks=haveStocks)
