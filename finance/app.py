import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
# from operator import itemgetter

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

# check if the number is an integer


def is_integer(n):
    try:
        float(n)
    except ValueError:
        return False
    else:
        return float(n).is_integer()


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
    if request.method == "GET":
        # Query the database for essential information, and next determine the number of different companies
        portfolio = db.execute("SELECT symbol, amount FROM portfolio WHERE user_id = ?", session["user_id"])
        counter = len([ele for ele in portfolio if isinstance(ele, dict)])
        total = 0

        # Loop through the list to quote the price of each stock
        for i in range(counter):
            quoted = lookup(portfolio[i]["symbol"])
            if not quoted:
                return apology("Error querying the price of each stock", 400)
            # Add the other necessary fields to that company's dict
            portfolio[i]["name"] = quoted["name"]
            portfolio[i]["price"] = quoted["price"]
            portfolio[i]["holding"] = portfolio[i]["price"] * portfolio[i]["amount"]
            total += portfolio[i]["holding"]

        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        total += cash[0]["cash"]
        return render_template("index.html", portfolio=portfolio, cash=cash[0]["cash"], total=total)

    return apology("TODO")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # Store the value passed by the lookup function in a dict
        symbol = request.form.get("symbol")
        quoted = lookup(symbol)
        amount_shares = request.form.get("shares")
        if not quoted:
            return apology("Not found or left blank", 400)

        # Check if the value of shares is an integer
        if not is_integer(amount_shares):
            return apology("Number of shares must be an integer", 400)

        amount_shares = int(amount_shares)
        if amount_shares < 0:
            return apology("Number of shares must be positive", 400)

        # Check if the cash present in the account is sufficient to complete the purchase and cast the share amount requested to an int
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        if cash[0]["cash"] < amount_shares * float(quoted["price"]):
            return apology("No money to do that", 400)
        else:
            # if the user has the cash in the account, subtract the total amount for the user account
            db.execute("UPDATE users SET cash = ? WHERE id = ?",
                       (cash[0]["cash"] - amount_shares * float(quoted["price"])), session["user_id"])
            # register the transaction details
            db.execute("INSERT INTO transactions (symbol, user_id, amount, transaction_type, price) VALUES (?, ?, ?, ?, ?)",
                       symbol, session["user_id"], amount_shares, "buy", float(quoted["price"]))
            # Check if the user has any share from that company, if yes insert a new line, else alter the amount of shares
            if not db.execute("SELECT * FROM portfolio WHERE user_id = ? AND symbol = ?",
                              session["user_id"], symbol):
                db.execute("INSERT INTO portfolio (user_id, symbol, amount) VALUES (?, ?, ?)",
                           session["user_id"], symbol, amount_shares)
            else:
                # Query the amount of shares this user had before the purchase
                pre_shares = db.execute("SELECT amount FROM portfolio WHERE user_id = ? AND symbol = ?", session["user_id"], symbol)
                amount_shares += pre_shares[0]["amount"]
                # the previous line can be done in sql I think, or update the new value in a new line
                db.execute("UPDATE portfolio SET amount = ? WHERE user_id = ? AND symbol = ?",
                           amount_shares, session["user_id"], symbol)

        # Redirect user to home page
        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    if request.method == "GET":
        # Query the database for essential information, and next determine the number of transactions made
        transactions = db.execute("SELECT * FROM transactions WHERE user_id = ?", session["user_id"])

        return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)
        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 400)

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
    if request.method == "GET":
        return render_template("quote.html")
    # Store the value passed by the lookup function in a dict
    quoted = lookup(request.form.get("symbol"))
    if not quoted:
        return apology("Not found of left blank", 400)
    return render_template("quoted.html", name=quoted["name"], price=quoted["price"], symbol=quoted["symbol"])


@app.route("/quoted", methods=["POST"])
@login_required
def quoted():
    if request.method == "POST":
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # Forget any user_id
    session.clear()

    # if the user reached route from POST method, run through the register process
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        # Ensure the username was submitted by the user
        if not username:
            return apology("Must provide username", 400)

        # Ensure the password was submitted by the user
        if not password or password != confirmation:
            return apology("Must provide password twice, and they have to match", 400)

        # Query the database for that user name, and save the values of that query in a dict/list called rows
        users = db.execute("SELECT * FROM users WHERE username = ?", username)

        # Ensure the user selects a username not already in use by another user
        for i in range(len(users)):
            if username == users[i]["username"]:
                return apology("Invalid username provided, choose another", 400)

        # Check to see if that username is valid
        if len(users) == 0:
            db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, generate_password_hash(password))
        else:
            apology("Invalid username or password provided", 400)
        users = db.execute("SELECT * FROM users WHERE username = ?", username)

        # Remember which user has logged in
        session["user_id"] = users[0]["id"]

        # Redirect user to home page
        return redirect("/")
    else:

        # return to the register root
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    companies = db.execute("SELECT symbol, amount FROM portfolio WHERE user_id = ?", session["user_id"])
    if request.method == "POST":
        # Do your thing 21
        amount_shares = int(request.form.get("shares"))
        symbol = request.form.get("symbol")
        quoted = lookup(symbol)

        # Loop through the list of companies to check if the pretended company is present
        for i in range(len(companies)):
            if symbol == companies[i]["symbol"]:
                # Check if the user has enough shares to sell
                if amount_shares > int(companies[i]["amount"]):
                    return apology("Not enough shares in possession", 400)
                else:
                    # Register in both the transactions and portfolio tables, the consequent sell
                    price = float(quoted["price"])
                    db.execute("INSERT INTO transactions (symbol, user_id, amount, transaction_type, price) VALUES (?, ?, ?, ?, ?)",
                               symbol, session["user_id"], amount_shares, "sell", price)
                    db.execute("UPDATE portfolio SET amount = ? WHERE user_id = ? AND symbol = ?",
                               (companies[i]["amount"] - amount_shares), session["user_id"], symbol)
                    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
                    new_cash = cash[0]["cash"] + amount_shares * price
                    # Update the users cash balance
                    db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, session["user_id"])
                    if companies[i]["amount"] - amount_shares == 0:
                        db.execute("DELETE FROM portfolio WHERE user_id = ? AND symbol = ? AND amount = ?",
                                   session["user_id"], symbol, "0")

        return redirect("/")
    else:
        return render_template("sell.html", companies=companies)
