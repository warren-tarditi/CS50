import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        self.positives = set ()
        self.negatives = set ()

        file = open (positives, 'r')
        for line in file:
            if line.startswith(';'):
                continue
            else:
                self.positives.add(line.strip())

        file = open (negatives, 'r')
        for line in file:
            if line.startswith(';'):
                continue
            else:
                self.negatives.add(line.strip())

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""

        score = 0
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)

        for word in tokens:
            word.lower()
            if word in self.positives:
                score = score + 1
            elif word in self.negatives:
                score = score - 1

        return score
